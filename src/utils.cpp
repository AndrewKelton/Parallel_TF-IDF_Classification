#include "utils.h"

extern const string utils_error_msg(const string msg, ERR_CODES err) {
    return "Error Code: " + to_string(err) + " " + msg;
}