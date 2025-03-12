/**
 * @file document.h
 * @author Andrew Kelton
 * @brief Defines the Document and Corpus classes and their related functions.
 * @version 0.1
 * @date 2025-03-12
 */

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <atomic>
#include <cmath>
#include <cstdarg>
#include <sstream>
#include <fstream>
#include "categories.h"
#include "utils.h"

using namespace std;

const string DOC_FILENAME{"test-output/lengthy/document-info.txt"};
const string COR_FILENAME{"test-output/lengthy/corpus-info.txt"};
class Category; // forward declaration

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

        int document_id{0};                           ///< Unique document identifier (for testing/debugging)
        string text;                                  ///< Raw text content of the document
        unordered_map<string, int> term_count;        ///< Term occurrence count within the document
        unordered_map<string, double> term_frequency; ///< Normalized term frequencies
        unordered_map<string, double> tf_idf;         ///< TF-IDF scores for terms in the document
        int total_terms{0};                           ///< Total number of words in the document
        text_cat_types_ category;                     ///< Classification category assigned to the document

        /**
         * @brief Checks whether a given term exists in the document.
         * @param str The term to search for.
         * @return True if the term is found, otherwise false.
         */
        bool is_term(string str);

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
        double calculate_term_frequency(string term);

        /* Helper functions for formatted output */
        string print_text() const;
        string print_number_terms() const;
        string print_category() const;
        string print_tf_idf() const;
        string print_doc_term_count() const;
};

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

        vector<Document> documents;                               ///< Collection of document objects in the corpus.
        unordered_map<string, double> inverse_document_frequency; ///< Stores the inverse document frequency (IDF) values.
        atomic<int> num_of_docs{0};                               ///< Total number of documents in the corpus.
        // unordered_map<string, int> document_frequency;         // # of documents each word appears in

        /**
        * @brief Computes the TF-IDF values for all documents in parallel.
        * 
        * @details This function calculates the Term Frequency-Inverse Document Frequency (TF-IDF) 
        * for each term in the corpus using multithreading to improve performance.
        */
        void tfidf_documents();

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
        int get_num_unique_terms();

        /**
         * @brief Prints corpus statistics to a file.
         * 
         * @details Writes details such as the number of documents, threads used, 
         * and documents processed per thread to an output file. If the file 
         * cannot be opened, an exception is thrown.
         */
        void print_all_info();

    private: 
    
        unsigned num_doc_per_thread; ///< Number of documents processed per thread.

        /**
         * @brief Returns the number of documents that contain a given term.
         * @param str The term to check.
         * @return The count of documents containing the term.
         */
        int num_doc_term(const string& str);

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
        void emplace_tfidf_document(Document * document);

        /**
         * @brief Determines the optimal number of documents per thread for parallel processing.
         * @return The computed number of documents to assign per thread.
         */
        unsigned get_number_of_docs_per_thread();

        /** @brief Returns formatted number of threads used for processing. */
        string print_number_threads_used() const;

        /** @brief Returns formatted number of documents assigned per thread. */
        string print_number_documents_per_thread() const;

        /** @brief Returns formatted total number of documents in the corpus. */
        string print_number_documents() const;
};

#endif // DOCUMENT_H