/**
 * @defgroup HeaderFiles Header Files
 * @brief Group containing all header files.
 * @{
 */

/**
 * @file utils.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Header-only utility library with common data types and functions.
 * 
 * @details This file provides utility functions and type definitions 
 * used throughout the project, including:
 * - Category and section management.
 * - Error handling and runtime exceptions.
 * - Time measurement utilities.
 * - Thread-related constants
 * 
 * @version 1.0
 * @date 2025-03-12
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <map>
#include <set>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdarg>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <thread>
#include <string>
#include <cstring>  // Required for strerror
#include <cerrno>   // Required for errno

/** @brief Maximum number of processing sections. */
#define MAX_SECTIONS 4

#define DEFAULT_TRAINED_INPUT_FILE "data/BBC-News-Training.csv"
#define DEFAULT_UN_TRAINED_INPUT_FILE "data/unknown_text.txt"
#define DEFAULT_UN_TRAINED_CORRECT_INPUT_FILE "data/correct_unknown.txt"
#define DEFAULT_PROCESSED_DATA_OUTPUT_CSV_FILE "processed.csv"


/** @brief Maximum number of threads supported by the system. */
inline const unsigned NUMBER_OF_THREADS_MAX{std::thread::hardware_concurrency()};

/** 
 * @enum class_type_
 * @brief Represents the different high-level classes in the project.
 */
enum class_type_ {
    _corpus_, _document_, _category_
};

/** 
 * @enum text_cat_types_
 * @brief Represents different text classification categories.
 */
enum text_cat_types_ {
    sport_t_,          ///< Sports category.
    business_t_,       ///< Business category.
    politics_t_,       ///< Politics category.
    tech_t_,           ///< Technology category.
    entertainment_t_,  ///< Entertainment category.
    invalid_t_         ///< Invalid, Unrecognized, or Unititialized category.
};

/** 
 * @brief Maps category names (strings) to their corresponding `text_cat_types_` enum.
 */
const std::map<std::string, text_cat_types_> categories_text = {
    {"sport", sport_t_},
    {"business", business_t_},
    {"politics", politics_t_},
    {"tech", tech_t_},
    {"entertainment", entertainment_t_}
};

/** 
 * @brief Maps `text_cat_types_` enums to their corresponding string representations.
 */
const std::map<text_cat_types_, std::string> text_categories = {
    {sport_t_, "sport"},
    {business_t_, "business"},
    {politics_t_, "politics"},
    {tech_t_, "tech"},
    {entertainment_t_, "entertainment"},
    {invalid_t_, "invalid"}
};

/** 
 * @enum section_type_
 * @brief Represents different computational sections used for processing.
 */
enum section_type_ {
    vectorization_, ///< Text vectorization step.
    tfidf_,         ///< TF-IDF computation step.
    categories_,    ///< Category classification step.
    unknown_        ///< Unknown classification step.
};

/** 
 * @brief Names of the different computation sections.
 */
inline static const std::string SECTION_NAME[4] = {
    "Vectorization", "TF-IDF", "Categories", "Unknown Classification"
};

/**
 * @brief Throws a formatted runtime error message.
 * 
 * @tparam Args Variadic template for formatted message components.
 * @param args The values to be formatted into the error message.
 * @throws std::runtime_error with the constructed message.
 */
template<typename... Args>
inline void throw_runtime_error(Args... args) {
    std::ostringstream err_response;

    ((err_response << args << " "), ...);

    std::string err_msg = err_response.str();
    
    throw std::runtime_error(err_msg);
}

/**
 * @brief Returns the name of a computation section.
 * 
 * @param type The section type as `section_type_`.
 * @return Corresponding section name as a string.
 */
inline std::string get_section_name(section_type_ type) {
    return SECTION_NAME[static_cast<int>(type)];
}


/** @brief Type alias for a time point. */
using _time_point_ = std::chrono::_V2::system_clock::time_point;

/**
 * @brief Calculates elapsed time in milliseconds.
 * 
 * @param start The start time.
 * @param end The end time.
 * @return Elapsed time in milliseconds.
 */
inline double elapsed_time_ms(_time_point_ start, _time_point_ end) {
    return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
}
// inline double elapsed_time_ms(std::chrono::_V2::system_clock::time_point start, std::chrono::_V2::system_clock::time_point end) {
//     return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
// }

/**
 * @brief Prints elapsed time in milliseconds.
 * 
 * @param start The start time.
 * @param end The end time.
 */
inline void print_elapsed_time_ms(_time_point_ start, _time_point_ end) {
    std::cout << elapsed_time_ms(start, end) << " ms" << std::endl;
}

/**
 * @brief Prints elapsed time for a specific computation section.
 * 
 * @param start The start time.
 * @param end The end time.
 * @param type The section type.
 */
inline void print_duration_code(_time_point_ start, _time_point_ end, section_type_ type) {
    std::cout << get_section_name(type) << ": ";
    print_elapsed_time_ms(start, end);
}

inline void print_duration_code(double duration, section_type_ type) {
    std::cout << get_section_name(type) << ": " << duration << " ms" << std::endl;
}

/**
 * @brief Converts an integer to `text_cat_types_` enum.
 * 
 * @param i Integer representing the category.
 * @return Corresponding `text_cat_types_` enum value.
 */
inline text_cat_types_ conv_cat_type(int i) {
    if (i > 5)
        return invalid_t_;
    return static_cast<text_cat_types_>(i);
}

/**
 * @brief Converts a `text_cat_types_` enum to its string representation.
 * 
 * @param type_ The category type enum.
 * @return Corresponding category name as a string.
 */
inline std::string conv_cat_type(text_cat_types_ type_) {
    try {
        return text_categories.at(type_);
    } catch (const std::out_of_range& e) {
        return text_categories.at(invalid_t_);
    }
}

/**
 * @brief Converts a string to a `text_cat_types_` enum.
 * 
 * @param str Category name as a string.
 * @return Corresponding `text_cat_types_` enum.
 */
inline text_cat_types_ conv_cat_type(std::string str) {
    return categories_text.at(str);
}

/** @} */ // End of UsageTestFiles group

#endif // _UTILS_HPP
