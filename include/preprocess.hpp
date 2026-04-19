/**
 * @file preprocess.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Handles text preprocessing tasks such as stemming and text normalization.
 * 
 * @details This module provides functions for processing and normalizing text data 
 * in `Document` objects. It uses the Oleander Stemming Library to reduce words 
 * to their root forms (e.g., "enjoying" → "enjoy"). 
 * 
 * @see https://github.com/Blake-Madden/OleanderStemmingLibrary
 */

#ifndef _PREPROCESS_HPP
#define _PREPROCESS_HPP

#include "Document.hpp"

/**
 * @brief Applies text preprocessing to a document.
 * 
 * @details This function processes all text in the given `Document` object, 
 * performing tasks such as stemming and text normalization.
 * 
 * @param doc Pointer to the `Document` object to preprocess.
 */
extern std::vector<std::string> preprocess_tokenize(const docs::Document * doc);

#endif // _PREPROCESS_HPP