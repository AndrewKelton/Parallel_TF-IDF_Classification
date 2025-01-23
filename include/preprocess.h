#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <set>
#include "document.h"

using namespace std;

// preprocess all text in document
extern void preprocess_text(Document * doc);

#endif