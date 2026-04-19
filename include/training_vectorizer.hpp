/**
 * @file training_vectorizer.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Provides functions to vectorize a Corpus object.
 * 
 * @details This file declares functions to transform a `Corpus` into a numerical representation 
 * by counting term frequencies in each document. It supports both multi-threaded and 
 * sequential vectorization methods.
 * 
 * The multi-threaded approach distributes documents across multiple threads for improved 
 * performance, while the sequential approach processes documents one at a time.
 */

#ifndef _TRAINING_VECTORIZER_HPP
#define _TRAINING_VECTORIZER_HPP

#include "Document.hpp"
#include "Corpus.hpp"


extern void build_vocaulary(corpus::Corpus * corpus);



#endif // _TRAINING_VECTORIZER_HPP