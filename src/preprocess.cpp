#include "preprocess.h"

string preprocess_sentence(string str) {
    transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return tolower(c); });

    for (int i = 0; i < str.length(); i++) {
        
    }   

    return str;
}