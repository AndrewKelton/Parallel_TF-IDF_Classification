#include "count_vectorization.h"
#include "document.h"

// void count_words(const std::string& text, std::map<std::string, int>& global_count) {
//     std::map<std::string, int> local_count;
//     std::istringstream iss(text);
//     std::string word;
// 
//     while (iss >> word)
//         local_count[word]++;
// 
//     std::lock_guard<std::mutex> lock(mtx);
//     for (const auto& [word, count] : local_count)
//         global_count[word] += count;
// }


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
 */
static void count_words_doc(Document * doc) {
    istringstream iss(doc->text);
    string word;

    while (iss >> word) {
        if (STOPWORDS.count(word) == 0) {
            doc->term_count[word]++;
            doc->total_terms++;
        } else 
            doc->term_count.erase(word);
    }
}

static void vectorize_doc(Document * doc) {
    preprocess_text(doc);
    count_words_doc(doc);
    (*doc).calculate_term_frequency_doc();
}

extern void vectorize_corpus_threaded(Corpus * corpus) {
    vector<thread> threads;

    for (auto& document : (*corpus).documents) {
        threads.emplace_back(thread(vectorize_doc, &document));
        corpus->num_of_docs++;
    }

    for (auto& t : threads)
        t.join();
}