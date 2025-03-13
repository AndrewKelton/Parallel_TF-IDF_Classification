/**
 * @file flag_handler.hpp
 * 
 * @author Andrew Kelton
 * @brief Handles user print flags and triggers the appropriate print functions.
 * 
 * @details This file defines functions that control the output of detailed information 
 * about the `Corpus`, `Category`, and `Document` objects. The printing behavior 
 * is controlled via feature flags (`config.hpp`). Depending on the enabled flags, 
 * different sets of data are written to output files.
 * 
 * The main functions include:
 * - `print_terms_info()`: Prints TF-IDF and term-related information for a corpus.
 * - `print_cat_info()`: Outputs classification categories.
 * - `print_lengthy()`: Prints all detailed information about the corpus, documents, and categories.
 * - `handle_output_flags()`: Checks flags and calls the appropriate printing functions.
 * 
 * @version 1.0
 * @date 2025-03-12
 */

#ifndef _FLAG_HANDLER_HPP
#define _FLAG_HANDLER_HPP

#include "config.hpp" 
#include "document.hpp"
#include "categories.hpp"
#include "utils.hpp"
#include <filesystem>

/**
 * @brief Prints term-related information for a given corpus.
 * 
 * @details Calls `Corpus::print_all_info()` to save corpus-level statistical 
 * information, including TF-IDF and term distributions.
 * 
 * @param corp Pointer to the `Corpus` object.
 */
inline static void print_terms_info(corpus::Corpus * corp) {
    try {
        corp->print_all_info();
    } catch (std::runtime_error e) {
        std::cerr << "Error Printing Corpus: " << e.what() << std::endl;
    }
}

/**
 * @brief Prints classification category information.
 * 
 * @details This function ensures that the category output file is reset before writing.  
 * It then iterates through the list of categories and prints their details.
 * 
 * @param cats A vector of `Category` objects to print.
 */
inline static void print_cat_info(std::vector<cats::Category>& cats) {
    if (std::filesystem::exists(CAT_FILENAME))
        std::filesystem::remove(CAT_FILENAME);
    
    std::ofstream tmp_f{CAT_FILENAME};
    tmp_f.close();

    for (auto& cat : cats) {
        try {
            cat.print_all_info();
        } catch (std::runtime_error e) {
            std::cerr << "Error Printing " << conv_cat_type(cat.get_type()) << ": " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Prints detailed corpus, document, and category information.
 * 
 * @details This function first prints term information from the corpus.
 * It then iterates through each document and prints its details.
 * Finally, it calls `print_cat_info()` to output category data.
 * 
 * @param corp Pointer to the `Corpus` object.
 * @param docs A vector of `Document` objects to print.
 * @param cats A vector of `Category` objects to print.
 */
inline static void print_lengthy(corpus::Corpus * corp, std::vector<docs::Document> docs, std::vector<cats::Category>& cats) {
    print_terms_info(corp);

    if (std::filesystem::exists(DOC_FILENAME))
        std::filesystem::remove(DOC_FILENAME);

    std::ofstream tmp_f{DOC_FILENAME};
    tmp_f.close();

    for (auto& doc : docs) {
        try {
            doc.print_all_info();
        } catch (std::runtime_error e) {
            std::cerr << "Error Printing Document #" << std::to_string(doc.document_id) << ": " << e.what() << std::endl;
        }
    }

    print_cat_info(cats);
}

/**
 * @brief Determines which print functions to call based on enabled flags.
 * 
 * @details This function checks the flag settings in `config.hpp` and calls the appropriate 
 * printing functions:
 * - If `ENABLE_LENGTHY` is set, it prints all available data.
 * - If `ENABLE_TERMS_INFO` is set but `ENABLE_LENGTHY` is not, it prints term-related data.
 * - If `ENABLE_CATS_INFO` is set but `ENABLE_LENGTHY` is not, it prints category-related data.
 * 
 * @param corpus Reference to the `Corpus` object.
 * @param documents Reference to a vector of `Document` objects.
 * @param cat_vect Reference to a vector of `Category` objects.
 */
inline void handle_output_flags(corpus::Corpus& corpus, std::vector<docs::Document>& documents, std::vector<cats::Category>& cat_vect) {
    #if ENABLE_LENGTHY
    print_lengthy(&corpus, documents, cat_vect);
    #endif

    #if (!ENABLE_LENGTHY && ENABLE_TERMS_INFO)
    print_terms_info(&corpus);
    #endif

    #if (!ENABLE_LENGTHY && ENABLE_CATS_INFO)
    print_cat_info(cat_vect);
    #endif
}

#endif // _FLAG_HANDLER_HPP