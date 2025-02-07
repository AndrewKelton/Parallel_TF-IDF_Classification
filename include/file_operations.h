#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "document.h"

using namespace std;

// read csv as whole insert into document in corpus
extern void read_csv_to_corpus(Corpus& corpus, const string& file_name);

// write tfidf values of terms to csv from entire corpus
extern void write_to_csv_tfidf_corpus(Corpus corpus, const string& file_name);

#endif