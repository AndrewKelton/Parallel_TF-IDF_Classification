/**
 * @file document.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the Document and Corpus classes and their related functions.
 * 
 * @version 1.1
 * @date 2025-03-19
 * 
 * @par Changelog:
 * - Moved `get_number_of_docs_per_thread()` in corpus::Corpus from private -> public.
 * - Changed `get_number_of_docs_per_thread()` to a `const` function.
 * 
 */

#ifndef _DOCUMENT_HPP
#define _DOCUMENT_HPP

#include <atomic>
#include <cmath>
#include <cstdarg>
#include <sstream>
#include <fstream>
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
            text_cat_types_ category; ///< Classification category assigned to the document

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


/**
 * @namespace corpus
 * @brief Provides functionality for managing and analyzing a collection of documents (corpus).
 * 
 * @details This namespace includes the `Corpus` class, which represents a collection of `Document` objects
 * used for text analysis tasks. The class provides functionality for calculating Term Frequency-Inverse Document 
 * Frequency (TF-IDF) values, as well as managing the corpus metadata and statistics. It supports both parallel 
 * and sequential processing to efficiently compute the TF-IDF values across documents.
 * 
 * The `Corpus` class handles:
 * - Managing the collection of `Document` objects in the corpus.
 * - Computing the TF-IDF values for terms across the corpus.
 * - Supporting parallel, sequential, and thread-specific processing methods.
 * - Storing the inverse document frequency (IDF) values for terms.
 * - Printing statistics and metadata related to the corpus.
 * 
 * Methods such as `tfidf_documents()` and `tfidf_documents_seq()` are available for parallel and sequential
 * TF-IDF computations, respectively. The class also provides utilities for determining the number of unique terms,
 * calculating the number of documents containing a term, and printing corpus statistics to an output file.
 */
namespace corpus {

    /**
     * @class Corpus
     * @brief Represents a collection of documents (corpus) for text analysis.
     * 
     * @details This class manages a collection of `Document` objects and provides 
     * functionality for computing term frequency-inverse document frequency (TF-IDF) 
     * values across the corpus. It supports both **parallel** and **sequential** processing 
     * for efficiency. 
     * 
     * The corpus maintains:
     * - `documents` : A vector of `Document` objects that make up the corpus.
     * - `inverse_document_frequency` : Stores the inverse document frequency (IDF) values for terms.
     * - `num_of_docs` : The total number of documents in the corpus.
     * - `num_doc_per_thread` : Determines how many documents are processed per thread during parallel computation.
     * 
     * The class includes methods for:
     * - `tfidf_documents()` : Computes TF-IDF values using parallel processing.
     * - `tfidf_documents_seq()` : Computes TF-IDF values sequentially.
     * - `tfidf_documents_not_dynamic()` : Computes TF-IDF using a fixed number of threads, 
     *   with one thread per document.
     * - `get_num_unique_terms()` : Returns the number of unique terms in the corpus.
     * - `print_all_info()` : Saves corpus-related statistics to an output file.
     * 
     * Private utility functions handle tasks such as document frequency calculations, 
     * inverse document frequency (IDF) computation, and managing thread assignments.
     */
    class Corpus {

        public:

            std::vector<docs::Document> documents;                               ///< Collection of document objects in the corpus.
            std::unordered_map<std::string, double> inverse_document_frequency; ///< Stores the inverse document frequency (IDF) values.
            std::atomic<int> num_of_docs{0};                               ///< Total number of documents in the corpus.
            // unordered_map<string, int> document_frequency;         // # of documents each word appears in

            /**
            * @brief Computes the TF-IDF values for all documents in parallel.
            * 
            * @details This function calculates the Term Frequency-Inverse Document Frequency (TF-IDF) 
            * for each term in the corpus using multithreading to improve performance.
            */
            void tfidf_documents();

            /**
            * @brief Computes the TF-IDF values for all documents in parallel.
            * 
            * @details This function calculates the Term Frequency-Inverse Document Frequency (TF-IDF) 
            * for each term in the corpus using multithreading to improve performance.
            */
            void tfidf_documents(int num_threads);

            /**
             * @brief Computes the TF-IDF values sequentially (single-threaded).
             * 
             * @details This function computes the TF-IDF scores for each document 
             * without utilizing parallel processing, making it suitable for debugging 
             * or small datasets.
             */
            void tfidf_documents_seq();

            /**
             * @brief Computes the TF-IDF values using one thread per document.
             * 
             * @details Unlike `tfidf_documents()`, this function assigns one active 
             * thread to process each document independently.
             * 
             * @warning This is NOT used in tests!! Use at your own risk, will cause 
             * high contention and excessive resource usage!!
             */
            void tfidf_documents_not_dynamic();
            
            /**
             * @brief Returns the number of unique terms in the corpus.
             * 
             * @return The count of unique terms across all documents.
             */
            int get_num_unique_terms() const;

            /**
             * @brief Prints corpus statistics to a file.
             * 
             * @details Writes details such as the number of documents, threads used, 
             * and documents processed per thread to an output file. If the file 
             * cannot be opened, an exception is thrown.
             * 
             * @throws std::runtime_error
             */
            void print_all_info();

            /**
             * @brief Determines the optimal number of documents per thread for parallel processing.
             * @return The computed number of documents to assign per thread.
             */
            unsigned get_number_of_docs_per_thread() const;

            /**
             * @brief Determines the optimal number of documents per thread for parallel processing.
             * @return The computed number of documents to assign per thread.
             */
            unsigned get_number_of_docs_per_thread(int num_of_threads) const;

        private: 

            unsigned num_threads_used{1}; ///< Number of threads used.
            unsigned num_doc_per_thread; ///< Number of documents processed per thread.

            /**
             * @brief Returns the number of documents that contain a given term.
             * @param str The term to check.
             * @return The count of documents containing the term.
             */
            int num_doc_term(const std::string& str);

            /**
             * @brief Computes the inverse document frequency (IDF) of a given term.
             * @param docs_with_term The number of documents containing the term.
             * @return The computed IDF value.
             * 
             * @details The IDF formula used:
             * \f$ IDF = \log{\frac{N}{df}} \f$
             * where:
             * - \( N \) is the total number of documents.
             * - \( df \) is the number of documents containing the term.
             */
            double idf_corpus(int docs_with_term);

            /**
             * @brief Computes and inserts the TF-IDF values for a document using a separate thread.
             * @param document Pointer to the `Document` object being processed.
             */
            void emplace_tfidf_document(docs::Document * document);

            /** @brief Returns formatted number of threads used for processing. */
            std::string print_number_threads_used() const;

            /** @brief Returns formatted number of documents assigned per thread. */
            std::string print_number_documents_per_thread() const;

            /** @brief Returns formatted total number of documents in the corpus. */
            std::string print_number_documents() const;
    };

} // namespace corpus

#endif // _DOCUMENT_HPP