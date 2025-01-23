#include "preprocess.h"

// convert uppercase text to lower case text
static string preprocess_to_lower_text(string str) {
    transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return tolower(c); });

    return str;
}

// remove punctuation from text
static string preprocess_remove_punc_text(string str) {
    string processed = "";

    for (char c : str)
        if (!ispunct(c) && !isnumber(c))
            processed += c;

    return processed;
}

void preprocess_text(Document * doc) {
    doc->text = preprocess_to_lower_text(preprocess_remove_punc_text(doc->text));
}