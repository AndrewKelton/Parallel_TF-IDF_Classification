#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <set>
#include <locale>
#include <codecvt>
#include <cctype>
#include "document.h"
#include "utils.h"
#include "english_stem.h"

using namespace std;

/* prune text, such as: "enjoying" -> "enjoy" 
 * uses the OleanderStemmingLibrary by Blake Madden
 * URL: https://github.com/Blake-Madden/OleanderStemmingLibrary.git
 */
extern string preprocess_prune_term(string str);

// preprocess all text in document
extern void preprocess_text(Document * doc);

#endif