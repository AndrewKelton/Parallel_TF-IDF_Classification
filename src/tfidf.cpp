/* tfidf.cpp
 * source file for tfidf.hpp
 */

#include "tfidf.hpp"
#include "file_operations.hpp"
#include "training_vectorizer.hpp"
#include "computation.hpp"

std::vector<double> build_centroid() {

}

void tfidf::fit(std::string path) {
    read_csv_to_corpus(this->training_corpus, path);

    build_vocaulary(&this->training_corpus);

    // compute tf idf
    compute_tf_matrix_training(&this->training_corpus);
    compute_idf_vector_training(&this->training_corpus);
    compute_tfidf_matrix_training(&this->training_corpus);

    // build categories centroids
    for (auto& category: this->)
}

void tfidf::transform(std::string path) {

}

void tfidf::classify() {

}