#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "document.h"

using namespace std;


/* prune text, such as: "enjoying" -> "enjoy" 
 * uses the OleanderStemmingLibrary by Blake Madden
 * URL: https://github.com/Blake-Madden/OleanderStemmingLibrary.git
 */
extern string preprocess_prune_term(string str);

// preprocess all text in document
extern void preprocess_text(Document * doc);

#endif