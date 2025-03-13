/**
 * @file preprocess.hpp
 * 
 * @author Andrew Kelton
 * @brief Handles text preprocessing tasks such as stemming and text normalization.
 * 
 * @details This module provides functions for processing and normalizing text data 
 * in `Document` objects. It uses the Oleander Stemming Library to reduce words 
 * to their root forms (e.g., "enjoying" → "enjoy"). 
 * 
 * @version 1.0
 * @date 2025-03-12
 * @see https://github.com/Blake-Madden/OleanderStemmingLibrary
 */

#ifndef _PREPROCESS_HPP
#define _PREPROCESS_HPP

#include "document.hpp"

/**
 * @brief Reduces a given word to its root form using stemming.
 * 
 * @details This function applies stemming to remove suffixes and normalize 
 * word forms. It helps in text preprocessing by converting different 
 * word variations into a consistent base form.
 * 
 * Example transformation:
 * \dot
 * digraph Stemming {
 *     node [shape=ellipse, style=filled, color=lightblue];
 *     "enjoying" -> "enjoy" [label="prune suffix '-ing'"];
 *     "running" -> "run" [label="prune suffix '-ing'"];
 *     "happier" -> "happy" [label="convert to root"];
 * }
 * \enddot
 * 
 * @param str The word to be stemmed.
 * @return The stemmed version of the input word.
 */
extern std::string preprocess_prune_term(std::string str);

/**
 * @brief Applies text preprocessing to a document.
 * 
 * @details This function processes all text in the given `Document` object, 
 * performing tasks such as stemming and text normalization.
 * 
 * @param doc Pointer to the `Document` object to preprocess.
 */
extern void preprocess_text(docs::Document * doc);

#endif // _PREPROCESS_HPP