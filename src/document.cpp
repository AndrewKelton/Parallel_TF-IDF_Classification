/* document.cpp
 * source file for document.h
 */

#include "document.h"
#include "categories.h"

bool Document::is_term(string str) {
    return term_count.find(str) != term_count.end();
}

double Document::calculate_term_frequency(string term) {
    if (total_terms == 0) 
        return 0.0;

    return static_cast<double>(term_count[term]) / total_terms;
}

void Document::calculate_term_frequency_doc() {
    for (auto& [word, count] : term_count) 
        term_frequency[word] = calculate_term_frequency(word);
}

double Corpus::idf_corpus(int docs_with_term) {
    return log(static_cast<double>(num_of_docs) / static_cast<double>(docs_with_term));
}

int Corpus::num_doc_term(string str) {
    int count{0};

    for (auto& d : documents) 
        if (d.is_term(str))
            count++;

    return count;
}

void Corpus::tfidf_documents() {
    vector<thread> threads;
    threads.reserve(NUMBER_OF_THREADS_MAX);

    unsigned number_of_docs_in_thread = get_number_of_docs_per_thread();
    unsigned number_of_docs_in_last_thread = num_of_docs % number_of_docs_in_thread;

    /* every 10 documents gets their own thread!!
     * done to reduce contention and reduce resource waste.
     */
    for (int i = 0; i < num_of_docs; i+=number_of_docs_in_thread) {
        threads.emplace_back([this, i, number_of_docs_in_thread, number_of_docs_in_last_thread]() {
            if (i == num_of_docs - number_of_docs_in_thread && number_of_docs_in_last_thread > 0) {
                for (int x = 0; x < number_of_docs_in_last_thread; x++) 
                    emplace_tfidf_document(&documents[x+i]);
            } else {
                for (int x = 0; x < number_of_docs_in_thread; x++) 
                    emplace_tfidf_document(&documents[x+i]);
            }
        });
    }

    for (auto& t : threads)
        t.join();
}

void Corpus::tfidf_documents_not_dynamic() {
    vector<thread> threads;

    /* every 10 documents gets their own thread!!
     * done to reduce contention and reduce resource waste.
     */
    for (int i = 0; i < num_of_docs; i+=10) {
        threads.emplace_back([this, i]() {
            for (int x = 0; x < 10; x++) 
                emplace_tfidf_document(&documents[x+i]);
        });
    }
}

// sequential
void Corpus::tfidf_documents_seq() {
    for (auto& document : documents) 
        emplace_tfidf_document(&document);
}

// using a thread insert tfidf into document. 
void Corpus::emplace_tfidf_document(Document * document) {
    for (const auto& [word, freq] : document->term_frequency)
        document->tf_idf[word] = freq * idf_corpus(num_doc_term(word));
}

int Corpus::get_num_unique_terms() {
    return inverse_document_frequency.size();
}

unsigned Corpus::get_number_of_docs_per_thread() {
    if (num_of_docs < NUMBER_OF_THREADS_MAX) 
        num_of_docs;

    return static_cast<unsigned>(num_of_docs) / NUMBER_OF_THREADS_MAX;
}

template <typename... Args>
void Corpus::test_print_private_funcs(Args... to_print) {
    (cout << ... << to_print) << endl;
}