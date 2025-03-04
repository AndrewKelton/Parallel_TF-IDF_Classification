#ifndef COUNT_VECTORIZATION_H
#define COUNT_VECTORIZATION_H

#include "document.h"

using namespace std;

// vectorize corpus of documents simultaneously
extern void vectorize_corpus_threaded(Corpus * corpus);

// vectorize corpus of documents sequentially
extern void vectorize_corpus_sequential(Corpus * corpus);

#endif