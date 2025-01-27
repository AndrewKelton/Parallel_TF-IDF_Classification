#include "tfidf.h"

mat construct_tfidf_matrix(const Corpus& corpus) {
    int num_docs = corpus.documents.size();
    int num_terms = corpus.inverse_document_frequency.size();

    // Create a matrix to hold TF-IDF values
    mat tfidf_matrix(num_docs, num_terms, arma::fill::zeros);

    // Map terms to column indices
    unordered_map<string, int> term_to_index;
    int index = 0;
    for (const auto& [term, idf] : corpus.inverse_document_frequency) {
        term_to_index[term] = index++;
    }

    // Fill the matrix with TF-IDF values
    for (int i = 0; i < num_docs; ++i) {
        const auto& document = corpus.documents[i];
        for (const auto& [term, tfidf_value] : document.tf_idf) {
            if (term_to_index.count(term)) {
                int col = term_to_index[term];
                tfidf_matrix(i, col) = tfidf_value;
            }
        }
    }

    return tfidf_matrix;
}

Row<size_t> extract_labels(const Corpus& corpus) {
    Row<size_t> labels(corpus.documents.size());
    for (size_t i = 0; i < corpus.documents.size(); ++i) {
        labels[i] = static_cast<size_t>(corpus.documents[i].category);
    }
    return labels;
}