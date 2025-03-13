/**
 * @file categories.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Contains Category class and functions to classify Document(s).
 * 
 * @details This file defines the `Category` class, which represents the classification categories
 * used to categorize documents in the corpus. The class provides various functions for managing
 * categories, including adding documents to a category, retrieving category information, and
 * performing classification tasks. 
 * 
 * The `Category` class is integral to the document classification system, helping organize and
 * classify documents based on predefined categories. The classification process can be based on
 * different criteria, such as the content of the documents or manually assigned labels. This file 
 * also contains utility functions for managing categories, checking if a document belongs to a 
 * specific category, and handling category-related data efficiently.
 * 
 * @version 1.0
 * @date 2025-03-12
 */

#ifndef _CATEGORIES_HPP
#define _CATEGORIES_HPP

#include <vector>
#include <unordered_map>
#include <fstream>
#include "utils.hpp"

#define MAX_CATEGORIES 5


/**
 * @namespace corpus
 * @brief Forward declarations for `corpus::Corpus`
 */
namespace corpus {
    class Corpus; // forward declaration
}

/**
 * @namespace cats
 * @brief Provides functionality for classifying documents into categories based on their content.
 * 
 * @details This namespace includes classes, structs, and functions to classify documents into predefined
 * categories by analyzing their terms using techniques like term frequency-inverse document frequency (TF-IDF). 
 * It offers parallel and sequential methods for determining the most important terms for each category, 
 * as well as functions to manage classification results and print them in a readable format.
 * 
 * The key class in this namespace is `Category`, which encapsulates the logic for computing, storing, 
 * and retrieving the most important terms for each category. The namespace also provides utilities for
 * performing classification of documents and managing classification accuracy.
 */
namespace cats { 

    const std::string CAT_FILENAME = "test-output/lengthy/category-info.txt";

    /**
     * @class Category
     * @brief Represents a category for classifying documents based on their content.
     * 
     * @details The `Category` class is responsible for determining and managing the category of a document 
     * by analyzing its terms and computing the term frequency-inverse document frequency (TF-IDF) 
     * scores for the terms. It maintains the most important terms in the category, which are ranked 
     * by their TF-IDF values.
     * 
     * The class allows retrieval of the most important terms, manages TF-IDF scores for all documents 
     * in the category, and supports efficient sorting and querying of the terms.
     * 
     * @note This class also handles serialization of category data to an output file for later use.
     */
    class Category {

        private:

            text_cat_types_ category_type;                     ///< Category type (text_cat_types_)
            int number_of_docs;                                ///< Number of documents in this category
            std::vector<std::pair<std::string, double>> most_important_terms; ///< List of top terms in the category sorted by TF-IDF
        
            /**
             * @brief Sorts the terms of the category by their TF-IDF value in descending order.
             * 
             * This method sorts the given unordered map of terms based on their TF-IDF values.
             * 
             * @param terms An unordered map of terms and their corresponding TF-IDF scores.
             * @return A sorted vector of term-TF-IDF pairs.
             */
            std::vector<std::pair<std::string, double>> sort_unordered_umap(std::unordered_map<std::string, double> terms);

            /**
             * @brief Retrieves the nth most important term for the category.
             * 
             * This function searches for the nth most important term, excluding previously used terms.
             * 
             * @param all_tfidf_terms A vector containing TF-IDF terms for all documents in the category.
             * @param used A list of previously used terms to avoid duplicates.
             * @return The nth most important term and its TF-IDF score.
             */
            std::pair<std::string, double> search_nth_important_term(std::vector<std::vector<std::pair<std::string, double>>> all_tfidf_terms, std::vector<std::pair<std::string, double>> used);

            /**
             * @brief Stores the TF-IDF values of all terms for the category.
             * 
             * This method processes the TF-IDF values of the given document and saves them in the 
             * `tf_idf_all` member.
             * 
             * @param doc_tf_idf A map of terms and their corresponding TF-IDF values for the document.
             */
            void put_tf_idf_all(std::unordered_map<std::string, double> doc_tf_idf);

        public:
            std::unordered_map<std::string, double> tf_idf_all; ///< TF-IDF terms of all documents in the category

            /**
             * @brief Prints all the important information for the category.
             * 
             * This function prints detailed information about the category, including the most important
             * terms and their TF-IDF scores, to an output file.
             */
            void print_all();

            /**
             * @name Constructors & Operators
             * @{
             */
                    
            /**
             * @brief Regular constructor to create a Category from the category type.
             * 
             * @param category_type The type of category (integer that maps to text_cat_types_).
             */
            Category(int category_type) : category_type{static_cast<text_cat_types_>(category_type)} {}

            /** 
             * @brief Copy Constructor 
             */ 
            Category(const Category&) = default;

            /** 
             * @brief Copy Assignment Operator 
             */
            Category& operator=(const Category&) = delete;  

            /** 
             * @brief Move Constructor 
             */
            Category(Category&& other) noexcept
                : category_type{other.category_type},
                most_important_terms{std::move(other.most_important_terms)},
                tf_idf_all{std::move(other.tf_idf_all)}  // Move tf_idf_all!
            {}

            /** 
             * @brief Move Assignment Operator 
             */
            Category& operator=(Category&& other) noexcept {
                if (this != &other) {
                    category_type = other.category_type;
                    most_important_terms = std::move(other.most_important_terms);
                    tf_idf_all = std::move(other.tf_idf_all);  // Move tf_idf_all!
                }
                return *this;
            }
            /**
             * @}
             */

            /**
             * @brief Gets the category type.
             * 
             * @return The type of the category as a text_cat_types_.
             */
            text_cat_types_ get_type() {
                return category_type;
            }
        
            /**
             * @brief Computes the most important terms for the category based on the corpus.
             * 
             * This function calculates the important terms based on the term frequency-inverse 
             * document frequency (TF-IDF) values from the given `Corpus` and stores them in the
             * `most_important_terms` vector.
             * 
             * @param corpus The corpus of documents used for calculating TF-IDF.
             */
            void get_important_terms(const corpus::Corpus& corpus);

            /**
             * @brief Prints detailed information about the category to a file.
             * 
             * This function writes the category type and the most important terms to a file, 
             * including their TF-IDF scores.
             */
            void print_all_info(); 
    };


    /**
     * @struct Classified_S
     * @brief Represents the classification results for a document.
     * 
     * This struct holds information about the correct and classified categories of a document.
     */
    struct Classified_S {
        text_cat_types_ correct_type;    ///< The correct category of the document.
        text_cat_types_ classified_type; ///< The category the document was classified into.
        bool correct;                    ///< Whether the classification was correct.
    };
    using unknown_class = Classified_S;  ///< Use `unkown_class`, I dislike capitals.

    /**
     * @struct Classification_S
     * @brief Represents the results of a classification process for a set of documents.
     * 
     * @details This struct holds a collection of documents with their classification results and the 
     * overall classification accuracy.
     */
    struct Classification_S {
        std::vector<unknown_class> unknown_doc; ///< List of documents with classification results.
        int correct_count;                 ///< Number of correctly classified documents.
        int total_count;                   ///< Total number of documents classified.
        double correct_db;                 ///< Classification accuracy as a decimal (correct/total).
    };
    using unknown_classification_s = Classification_S;


    /**
     * @brief Get important terms for a Category using parallel processing (5 threads, 1 per Category).
     * 
     * This function computes the most important terms for a category using parallel processing,
     * with each thread handling one category type.
     * 
     * @param corpus The corpus of documents used for calculating TF-IDF.
     * @param cats A vector to store the resulting Category objects.
     * @param catint The category type to process.
     */
    extern void get_single_cat_par(const corpus::Corpus& corpus, std::vector<Category>& cats, text_cat_types_ catint);


    /**
     * @brief Get important terms for a Category using sequential processing.
     * 
     * This function computes the most important terms for a category sequentially, 
     * processing one category type at a time.
     * 
     * @param corpus The corpus of documents used for calculating TF-IDF. It must be a valid pointer to a `Corpus` object.
     * @param cats A vector to store the resulting Category objects. The categories will be filled with the most important terms.
     * @param catint The category type to process, represented as a value from `text_cat_types_`.
     */
    extern void get_single_cat_seq(const corpus::Corpus& corpus, std::vector<Category>& cats, text_cat_types_ catint);


    /**
     * @brief Initializes the classification process for a set of documents.
     * 
     * This function sets up the classification process, creating the necessary classification structures 
     * and comparing the documents in the unknown corpus against the given category types.
     * It returns the classification results, including the number of correctly classified documents and 
     * their respective categories.
     * 
     * @param unknown_corpus The corpus of documents to classify. It must be a valid pointer to a `Corpus` object.
     * @param cat_vect A vector of `Category` objects to compare against. It should contain the precomputed categories.
     * @param correct_types A vector of correct category labels corresponding to the documents in `unknown_corpus`.
     * @return A `Classification_S` struct containing the classification results, including the count of correct classifications.
     */
    extern unknown_classification_s init_classification(const corpus::Corpus& unknown_corpus, std::vector<Category> cat_vect, std::vector<std::string> correct_types);


    /**
     * @brief Classifies a single document into one of the categories.
     * 
     * This function compares a document’s terms (provided as a map of TF-IDF values) against 
     * the precomputed categories and classifies the document into the most appropriate category.
     * It also checks if the classification is correct by comparing it with the correct category.
     * 
     * @param unknownText An unordered map of terms and their corresponding TF-IDF values for the document.
     * @param cat_vect A vector of `Category` objects to compare against.
     * @param correct_type The correct category label for the document.
     * @return A `Classified_S` struct containing the classification results for the document.
     */
    extern unknown_class classify_text(const std::unordered_map<std::string, double>& unknownText, std::vector<Category> cat_vect, std::string correct_type);


    /**
     * @brief Prints the classification results for a set of documents.
     * 
     * This function prints the classification results for a set of documents, showing both 
     * the correct categories and the categories the documents were classified into. It also 
     * computes and displays the overall classification accuracy.
     * 
     * @param classifications The classification results for the set of documents, represented by an 
     *        `unknown_classification_s` structure.
     */
    extern void print_classifications(unknown_classification_s classifications);


    /**
     * @brief Deprecated function to get important terms for all categories.
     * 
     * This function was used to retrieve the important terms for all categories in the corpus. 
     * It has been replaced by more efficient methods of parallel and sequential processing. 
     * 
     * @param corpus The corpus of documents used for calculating TF-IDF.
     * @return A vector of `Category` objects, each containing the most important terms for its category.
     * 
     * @deprecated This is a deprecated function, please use `get_single_cat_par()` or `get_single_cat_seq()`.
     */
    extern std::vector<Category> get_all_category_important_terms(const corpus::Corpus& corpus);


} // namespace cats


#endif // _CATEGORIES_HPP