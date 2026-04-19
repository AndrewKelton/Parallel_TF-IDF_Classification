/**
 * @file Corpus.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the Corpus class and its related functions.
 */

#ifndef _CORPUS_HPP
#define _CORPUS_HPP

#include "Document.hpp"
#include "utils.hpp"
#include "categories.hpp"

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

            std::vector<docs::Document> documents;  ///< Collection of document objects in the corpus.
            std::unordered_map<std::string, double> inverse_document_frequency; ///< Stores the inverse document frequency (IDF) values.
            std::atomic<int> num_of_docs{0};    ///< Total number of documents in the corpus.
            std::atomic<int> num_of_categories{0};  ///< Total number of categories in the corpus.
            std::unordered_set<std::string> category_types_set; ///< set of category types as strings.

            std::vector<std::string> vocabulary; ///< index → term string
            std::unordered_map<std::string, int> term_to_index; ///< term string → index

            std::vector<std::vector<double>> tf_matrix; ///< rows = documents, cols = vocab terms
            std::vector<double> idf_vector; ///< A single vector of length vocab_size
            std::vector<std::vector<double>> tfidf_matrix; ///< rows = documents, cols = TF-IDF scores for each vocab term

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

#endif // _CORPUS_HPP