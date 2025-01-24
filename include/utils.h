#ifndef UTILS_H
#define UTILS_H

#include <iostream>

using namespace std;

enum ERR_CODES {
    
    BAD_CAST = 4,
    BAD_TEXT = 8,
    FILE_NOT_FOUND = 100
};

extern const string utils_error_msg(const string msg, int err);

#endif