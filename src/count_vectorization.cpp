#include "count_vectorization.h"
#include <thread>

atomic<int> doc_id_count{0}; // document id 

/* words that carry no value and are voided 
 * used from https://towardsdatascience.com/building-a-cross-platform-tfidf-text-summarizer-in-rust-7b05938f4507
*/
static const set<string> STOPWORDS{
    "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", 
    "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", 
    "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", 
    "theirs", "themselves", "what", "which", "who", "whom", "this", "that", 
    "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", 
    "have", "has", "had", "having", "do", "does", "did", "doing", "an", "the", 
    "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", 
    "by", "for", "with", "about", "against", "between", "into", "through", 
    "during", "before", "after", "above", "below", "to", "from", "up", "down", 
    "in", "out", "on", "off", "over", "under", "again", "further", "then", "oh",
    "once", "here", "there", "when", "where", "why", "how", "all", "any", 
    "both", "each", "few", "more", "most", "other", "some", "such", "no", 
    "nor", "not", "only", "own", "same", "so", "than", "too", "very", "can", 
    "will", "just", "don", "should", "now", "a", "b", "c", "d", "e", "f", 
    "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
    "v", "w", "x", "y", "z"
};

/* Increments term count in a Document.
 * Ignores words in STOPWORDS, does NOT 
 * remove them from the text. 
 * Also, prunes the text before checking 
 * against STOPWORDS. Pruning must be done
 * AFTER tokenizing a term.
 */
static void count_words_doc(Document * doc) {
    istringstream iss(doc->text);
    string word;

    while (iss >> word) {
        word = preprocess_prune_term(word);
        if (STOPWORDS.count(word) == 0) {
            doc->term_count[word]++;
            doc->total_terms++;
        } else 
            doc->term_count.erase(word);
    }
}

// preprocess and vectorize a document
static void vectorize_doc(Document * doc) {
    
    doc->document_id = doc_id_count.load(memory_order_acquire);
    doc_id_count.fetch_add(1, memory_order_release);

    preprocess_text(doc);
    count_words_doc(doc);
    (*doc).calculate_term_frequency_doc();
}

static void vectorize_doc_sequenital(Document * doc, int * id) {
    preprocess_text(doc);
    count_words_doc(doc);
    (*doc).calculate_term_frequency_doc();
}


// vectorize corpus of documents simultaneously
extern void vectorize_corpus_threaded(Corpus * corpus) {
    vector<thread> threads;

    for (auto& document : (*corpus).documents) {
        threads.emplace_back(thread(vectorize_doc, &document));
    }

    for (auto& t : threads)
        t.join();
    corpus->num_of_docs = doc_id_count.load();
}

extern void vectorize_corpus_sequential(Corpus * corpus) {
    int id = 0;

    for (auto& document : (*corpus).documents) {
        vectorize_doc(&document);
        document.document_id = id++;
        corpus->num_of_docs++;
    }
}

// // vectorize categorize of documents simultaneously
// extern void vectorize_corpus_category_threaded(Corpus * corpus, int category) {
//     int num_of_threads = MAX_CATEGORIES;
// 
//     thread threads[num_of_threads];
// 
//     for (int i = 0; i < num_of_threads; i++) {
//         
//     }   
// }