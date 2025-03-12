/**
 * @file count_vectorization.h
 * @author Andrew Kelton
 * @brief Provides functions to vectorize a Corpus object.
 * 
 * @details This file declares functions to transform a `Corpus` into a numerical representation 
 * by counting term frequencies in each document. It supports both multi-threaded and 
 * sequential vectorization methods.
 * 
 * The multi-threaded approach distributes documents across multiple threads for improved 
 * performance, while the sequential approach processes documents one at a time.
 * 
 * @version 0.1
 * @date 2025-03-12
 */

#ifndef COUNT_VECTORIZATION_H
#define COUNT_VECTORIZATION_H

#include "document.h"

using namespace std;


/**
 * @brief Vectorizes a corpus using multi-threading for faster processing.
 * 
 * This function converts each document in the given `Corpus` into a term frequency vector,
 * where each term in the document is counted and stored. Multi-threading is used to speed 
 * up the vectorization process by distributing documents across available CPU cores.
 * 
 * @param corpus Pointer to the `Corpus` object that contains the documents to be vectorized.
 * 
 * @note This function modifies the `Corpus` object in place by updating the vectorized 
 *       representation of each document.
 * @warning Ensure thread safety when accessing shared resources within `Corpus`.
 */
extern void vectorize_corpus_threaded(Corpus * corpus);

/**
 * @brief Vectorizes a corpus sequentially, processing one document at a time.
 * 
 * This function iterates through the `Corpus` and converts each document into a term 
 * frequency vector, storing the count of each term. Unlike the multi-threaded approach, 
 * this function processes the documents one after another.
 * 
 * @param corpus Pointer to the `Corpus` object that contains the documents to be vectorized.
 * 
 * @note This function modifies the `Corpus` object in place by updating the vectorized 
 *       representation of each document.
 */
extern void vectorize_corpus_sequential(Corpus * corpus);

#endif // COUNT_VECTORIZATION_H