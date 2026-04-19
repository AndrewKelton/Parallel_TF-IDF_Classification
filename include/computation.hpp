/**
 * @file computation.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the computation functions
 */

#include "Corpus.hpp"

extern void compute_tf_matrix_training(corpus::Corpus * corpus);

extern void compute_idf_vector_training(corpus::Corpus * corpus);

extern void compute_tfidf_matrix_training(corpus::Corpus * corpus);