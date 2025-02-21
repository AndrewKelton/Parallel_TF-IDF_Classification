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

enum ERR_CODES {
    BAD_CAST = 4,
    BAD_TEXT = 8,
    FILE_NOT_FOUND = 100
};

enum FLAG_TYPES {
    review_flag, article_flag, 
    print_flag, timer_flag
};

// extern const string utils_error_msg(const string msg, int err);

template<typename... Args>
inline void throw_runtime_error(Args... args) {
    ostringstream err_response;

    ((err_response << args << " "), ...);

    string err_msg = err_response.str();
    // if (!err_msg.empty()) 
    //     err_msg.pop_back();
    
    throw runtime_error(err_msg);
}

#endif