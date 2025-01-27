#ifndef UTILS_H
#define UTILS_H

#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

enum ERR_CODES {
    BAD_CAST = 4,
    BAD_TEXT = 8,
    FILE_NOT_FOUND = 100
};

enum FLAG_TYPES {
    review_t, article_t, 
    print_t, timer_t
};

// const unordered_map<string, int> flags = {
//     {"review", FLAG_TYPES::review_t},       // review type of data classification, 1 (positive) 0 (negative)
//     {"article", FLAG_TYPES::article_t},     // article type of data classification, 5 categories of classification
//     {"print", FLAG_TYPES::print_t},         // print all output
//     {"time", FLAG_TYPES::timer_t}            // print time 
// };

extern const string utils_error_msg(const string msg, int err);

#endif