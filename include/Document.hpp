/**
 * @file Document.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the Document its related functions.
 */

#ifndef _DOCUMENT_HPP
#define _DOCUMENT_HPP

#include <atomic>
#include <cmath>
#include <cstdarg>
#include <sstream>
#include <fstream>
#include <unordered_set>

#include "utils.hpp"
#include "categories.hpp"

class Category; ///< Forward declaration


/**
 * @namespace docs
 * @brief Provides functionality for representing and analyzing individual documents within a corpus.
 * 
 * @details This namespace contains the `Document` class, which represents a single document or article within a 
 * corpus for the purpose of text analysis. The class stores raw text, term counts, normalized term frequencies, 
 * and TF-IDF scores, and provides methods to perform operations on these values.
 * 
 * The `Document` class is responsible for:
 * - Storing the raw text and textual data of the document.
 * - Computing term frequencies and TF-IDF scores for terms within the document.
 * - Storing the classification category of the document.
 * - Providing utility functions to check for terms, calculate term frequencies, and output document-related information.
 * 
 * Key attributes of the `Document` class include:
 * - `text` : The raw text content of the document.
 * - `term_count` : A hashmap that stores the frequency of each term in the document.
 * - `term_frequency` : A hashmap that stores normalized term frequencies for each term.
 * - `tf_idf` : A hashmap that stores the TF-IDF scores of the terms in the document.
 * - `category` : The classification category assigned to the document.
 * - `total_terms` : The total number of words in the document.
 * 
 * The class provides methods such as `is_term()` to check if a specific term exists in the document, 
 * `calculate_term_frequency_doc()` to compute term frequencies for all words in the document, 
 * and `print_all_info()` to output document details to a file.
 */ 
namespace docs { 
    
    const std::string DOC_FILENAME{"tests/output/lengthy/document-info.txt"}; ///< Document lengthy info output file
    const std::string COR_FILENAME{"tests/output/lengthy/corpus-info.txt"};   ///< Corpus lengthy info output file

    /**
     * @class Document
     * @brief Represents a document or article within a corpus.
     * 
     * @details This class provides the structure for analyzing a document 
     * and storing its computed textual data. It maintains the raw text, 
     * word frequency counts, and TF-IDF scores for terms within the document.
     * 
     * The class also provides utility functions to compute term frequencies, 
     * check if a term exists in the document, and output document-related information.
     * 
     * Key attributes include:
     * - `text` : Stores the full text of the document.
     * - `term_count` : A hashmap storing the frequency of each term.
     * - `term_frequency` : A hashmap storing the normalized term frequencies.
     * - `tf_idf` : A hashmap storing the TF-IDF scores of terms.
     * - `category` : The assigned classification category of the document.
     * - `total_terms` : The total number of words in the document.
     * 
     * Methods include:
     * - `is_term(string str)`: Checks if a given term exists in the document.
     * - `calculate_term_frequency_doc()`: Computes term frequencies for all words.
     * - `print_all_info()`: Writes document information to an output file.
     */
    class Document {

        public:

            int document_id{0}; ///< Unique document identifier (for testing/debugging)
            std::string text;   ///< Raw text content of the document
            std::unordered_map<std::string, int> term_count;        ///< Term occurrence count within the document
            std::unordered_map<std::string, double> term_frequency; ///< Normalized term frequencies
            std::unordered_map<std::string, double> tf_idf;         ///< TF-IDF scores for terms in the document
            int total_terms{0};       ///< Total number of words in the document
            std::string category; ///< Classification category assigned to the document

            /**
             * @brief Checks whether a given term exists in the document.
             * @param str The term to search for.
             * @return True if the term is found, otherwise false.
             */
            bool is_term(std::string str);

            /**
             * @brief Calculates term frequency for all words in the document.
             * 
             * Populates the `term_frequency` map with normalized term frequencies, where:
             * \f$ tf(term) = \frac{\text{term occurrences}}{\text{total terms in document}} \f$
             */
            void calculate_term_frequency_doc();

            /**
             * @brief Prints all document details, including term counts and TF-IDF scores.
             */
            void print_all_info();

        private:

            /**
             * @brief Computes the term frequency of a specific word in the document.
             * @param term The word whose frequency is to be calculated.
             * @return The normalized term frequency.
             */
            double calculate_term_frequency(std::string term);

            /* Helper functions for formatted output */
            std::string print_text() const;
            std::string print_number_terms() const;
            std::string print_category() const;
            std::string print_tf_idf() const;
            std::string print_doc_term_count() const;
    };

} // namespace documents

#endif // _DOCUMENT_HPP