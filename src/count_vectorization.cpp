/* count_vectorization.cpp
 * source file for count_vectorization.hpp
 */

#include "count_vectorization.hpp"
#include "preprocess.hpp"
#include "categories.hpp"
#include "Corpus.hpp"
#include "constants/stopwords.hpp"
#include <unordered_set>

std::atomic<int> doc_id_count{0}; // document id 

// O(1) average lookup — built once at static init from constants/stopwords.hpp
static const std::unordered_set<std::string> STOPWORDS(
    STOPWORDS_LIST.begin(), STOPWORDS_LIST.end()
);

/* Increments term count in a Document.
 * Ignores words in STOPWORDS, does NOT 
 * remove them from the text. 
 * Also, prunes the text before checking 
 * against STOPWORDS. Pruning must be done
 * AFTER tokenizing a term.
 */
static void count_words_doc(docs::Document * doc) {
    std::istringstream iss(doc->text);
    std::string word;

    while (iss >> word) {
        word = preprocess_prune_term(word);
        if (STOPWORDS.count(word) == 0) {
            doc->term_count[word]++;
            doc->total_terms++;
        } else 
            doc->term_count.erase(word);
    }
}

// preprocess and vectorize a document (helper for threaded)
static void vectorize_doc_parallel(docs::Document * doc) {
    doc->document_id = doc_id_count.load(std::memory_order_acquire);
    doc_id_count.fetch_add(1, std::memory_order_release);

    preprocess_text(doc);
    count_words_doc(doc);
    (*doc).calculate_term_frequency_doc();
}


// preprocess and vectorize a document sequenitally
static void vectorize_doc_sequenital(docs::Document * doc) {
    preprocess_text(doc);
    count_words_doc(doc);
    (*doc).calculate_term_frequency_doc();
}


// main vectorization function for parallel execution
extern void vectorize_corpus_threaded(corpus::Corpus * corpus) {
    std::vector<std::thread> threads;

    for (auto& document : (*corpus).documents) {
        threads.emplace_back(std::thread(vectorize_doc_parallel, &document));
    }

    for (auto& t : threads)
        t.join();
    // corpus->num_of_docs.store(doc_id_count.load(memory_order_acquire));
}

// main vectorization function for parallel execution
extern void vectorize_corpus_threaded(corpus::Corpus * corpus, int num_threads) {
    std::vector<std::thread> threads;

    unsigned number_of_docs_in_thread = corpus->get_number_of_docs_per_thread(num_threads);
    unsigned number_of_docs_in_last_thread = corpus->num_of_docs % number_of_docs_in_thread;

    for (int i = 0; i < corpus->num_of_docs; i+=number_of_docs_in_thread) {
        threads.emplace_back([&corpus, i, number_of_docs_in_thread, number_of_docs_in_last_thread]() {
            if (i == corpus->num_of_docs - number_of_docs_in_thread && number_of_docs_in_last_thread > 0) {
                for (unsigned x = 0; x < number_of_docs_in_last_thread; x++) 
                    if (i + x < corpus->num_of_docs)
                        vectorize_doc_parallel(&(corpus->documents[x+i]));
            } else {
                for (unsigned x = 0; x < number_of_docs_in_thread; x++) 
                    if (i + x < corpus->num_of_docs)
                        vectorize_doc_parallel(&(corpus->documents[x+i]));
            }
        });
    }

    for (auto& t : threads)
        t.join();
    // corpus->num_of_docs.store(doc_id_count.load(memory_order_acquire));
}

// main vectorization function for sequential execution
extern void vectorize_corpus_sequential(corpus::Corpus * corpus) {
    int id = 0;

    for (auto& document : (*corpus).documents) {
        vectorize_doc_sequenital(&document);
        document.document_id = id++;
        corpus->num_of_docs++;
    }
}
