/* utils.h 
 *
 * A header-only utility library containing data types and
 * functions used across all source code in this project.
 */

#ifndef UTILS_H
#define UTILS_H

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

using namespace std;

#define MAX_SECTIONS 3

inline const unsigned NUMBER_OF_THREADS_MAX{thread::hardware_concurrency()};

enum class_type_ {
    _corpus_, _document_, _category_
};

/* -- Category Utilities -- */
enum text_cat_types_ {
    sport_t_, business_t_,
    politics_t_, tech_t_,
    entertainment_t_, invalid_t_
};

const map<string, text_cat_types_> categories_text = {
    {"sport", sport_t_},
    {"business", business_t_},
    {"politics", politics_t_},
    {"tech", tech_t_},
    {"entertainment", entertainment_t_}
};

const map<text_cat_types_, string> text_categories = {
    {sport_t_, "sport"},
    {business_t_, "business"},
    {politics_t_, "politics"},
    {tech_t_, "tech"},
    {entertainment_t_, "entertainment"},
    {invalid_t_, "invalid"}
};
/* -- Category Utilities -- */

enum section_type_ {
    vectorization_, tfidf_, categories_, unknown_
};

inline static const string SECTION_NAME[4] = {
    "Vectorization", "TF-IDF", "Categories", "Unknown Classification"
};

// throw special runtime error message
template<typename... Args>
inline void throw_runtime_error(Args... args) {
    ostringstream err_response;

    ((err_response << args << " "), ...);

    string err_msg = err_response.str();
    
    throw runtime_error(err_msg);
}

// return the name of the section we are in
inline string get_section_name(section_type_ type) {
    return SECTION_NAME[static_cast<int>(type)];
}

// shortened 'time_point'
using _time_point_ = std::chrono::_V2::system_clock::time_point;

// return formatted elapsed time
inline double elapsed_time_ms(_time_point_ start, _time_point_ end) {
    return static_cast<double>(chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
}

// print the elapsed time
inline void print_elapsed_time_ms(_time_point_ start, _time_point_ end) {
    cout << elapsed_time_ms(start, end) << " ms" << endl;
}

// print elapsed time of a named code segment
inline void print_duration_code(_time_point_ start, _time_point_ end, section_type_ type) {
    cout << get_section_name(type) << ": ";
    print_elapsed_time_ms(start, end);
}

// convert integer to text_cat_types
inline text_cat_types_ conv_cat_type(int i) {
    if (i > 5)
        return invalid_t_;
    return static_cast<text_cat_types_>(i);
}

// convert text_cat_types_ to string
inline string conv_cat_type(text_cat_types_ type_) {
    try {
        return text_categories.at(type_);
    } catch (const out_of_range& e) {
        return text_categories.at(invalid_t_);
    }
}

// string to category type
inline text_cat_types_ conv_cat_type(string str) {
    return categories_text.at(str);
}

#endif