#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <cstdarg>
#include <exception>
#include <stdexcept>
#include <sstream>

using namespace std;

#define MAX_SECTIONS 3

enum ERR_CODES {
    BAD_CAST = 4,
    BAD_TEXT = 8,
    FILE_NOT_FOUND = 100
};

enum FLAG_TYPES {
    review_flag, article_flag, 
    print_flag, timer_flag
};

enum section_type_ {
    vectorization_, tfidf_, categories_
};


static const string SECTION_NAME[3] = {
    "Vectorization", "TF-IDF", "Categories"
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

#endif