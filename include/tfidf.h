#ifndef TFIDF_H
#define TFIDF_H

#include <armadillo>
#include "document.h"

using namespace arma;

mat construct_tfidf_matrix(const Corpus& corpus);

Row<size_t> extract_labels(const Corpus& corpus);

#endif