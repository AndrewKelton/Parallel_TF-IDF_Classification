/**
 * @file count_vectorization.hpp
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
 * 
 * @version 1.0
 * @date 2025-03-12
 */

#ifndef _COUNT_VECTORIZATION_HPP
#define _COUNT_VECTORIZATION_HPP

#include "document.hpp"


/**
 * @brief Vectorizes a corpus using multi-threading for faster processing.
 * 
 * @details This function converts each document in the given `Corpus` into a term frequency vector,
 * where each term in the document is counted and stored. Multi-threading is used to speed 
 * up the vectorization process by distributing documents across available CPU cores.
 * 
 * @param corpus Pointer to the `Corpus` object that contains the documents to be vectorized.
 * 
 * @note This function modifies the `Corpus` object in place by updating the vectorized 
 *       representation of each document.
 * @warning Ensure thread safety when accessing shared resources within `Corpus`.
 */
extern void vectorize_corpus_threaded(corpus::Corpus * corpus);


/**
 * @brief Vectorizes a corpus sequentially, processing one document at a time.
 * 
 * @details This function iterates through the `Corpus` and converts each document into a term 
 * frequency vector, storing the count of each term. Unlike the multi-threaded approach, 
 * this function processes the documents one after another.
 * 
 * @param corpus Pointer to the `Corpus` object that contains the documents to be vectorized.
 * 
 * @note This function modifies the `Corpus` object in place by updating the vectorized 
 *       representation of each document.
 * 
 * @warning Will take a VERY long time to complete when using large databases.
 *      This function will use a large portion of your CPU power, I recommend
 *      not running this function locally.
 */
extern void vectorize_corpus_sequential(corpus::Corpus * corpus);


#endif // _COUNT_VECTORIZATION_HPP