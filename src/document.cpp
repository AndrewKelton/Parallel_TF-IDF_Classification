#include "document.h"

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

// void Corpus::calculate_frequency_corpus() {
//     for (auto& d : documents) {
//         for (auto& [word, count] : d.term_count) {
//             int count = 0;
//             
//         }
//     }
// }