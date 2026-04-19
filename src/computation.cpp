#include "computation.hpp"
#include <cmath>

extern void compute_tf_matrix_training(corpus::Corpus * corpus) {
    int document_idx{0};

    // sequential implementation
    for (auto& document: corpus->documents) {
        for (auto& term_count: document.term_count) {
            int col = corpus->term_to_index[term_count.first];
            corpus->tf_matrix[document_idx][col] = term_count.second / document.total_terms;
        }
        document_idx += 1;
    }
}

extern void compute_idf_vector_training(corpus::Corpus * corpus) {
    int total_terms_unique = corpus->get_num_unique_terms();
    int total_documents = corpus->num_of_docs;


    for (auto& term: corpus->vocabulary) {
        int document_frequency{0};
    
        for (auto& document: corpus->documents) {
            if (document.is_term(term)) {
                document_frequency += 1;
            }
        }
        int vector_index =  corpus->term_to_index[term];

        // note: we are smoothing here
        corpus->idf_vector[vector_index] = std::log(static_cast<double>(1 + total_documents) / (1+ document_frequency)) + 1;
    }
}

extern void compute_tfidf_matrix_training(corpus::Corpus * corpus) {
    int total_terms_unique = corpus->get_num_unique_terms();
    int total_documents = corpus->num_of_docs;

    for (int i = 0; i < total_documents; ++i) {
        for (auto& term: corpus->vocabulary) {
            int col = corpus->term_to_index[term];
            corpus->tfidf_matrix[i][corpus->term_to_index[term]] = corpus->tf_matrix[i][col] * corpus->idf_vector[corpus->term_to_index[term]];
        }
    }
}