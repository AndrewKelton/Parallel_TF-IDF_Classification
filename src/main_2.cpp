#include <chrono>
#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include "file_operations.h"
#include <mlpack.hpp>
#include "tfidf.h"
#include <mlpack/methods/logistic_regression/logistic_regression.hpp>

using namespace std;

int main() {

    Corpus corpus;
    read_csv_to_corpus(ref(corpus), "data/bbc-text.csv");
    // cout << "read file, # of docs: " << corpus.num_of_docs << endl;
    vectorize_corpus_threaded(&corpus);
    cout << "Done vectorizing" << endl;

    auto start = chrono::high_resolution_clock::now();
    corpus.tfidf_documents();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end - start;
    cout << "Done creating tfidf, time: " << elapsed_time.count() << endl;

    mat X_train = construct_tfidf_matrix(ref(corpus));
    Row<size_t> y_train = extract_labels(corpus);

    mlpack::regression::LogisticRegression<> classifier(X_train, y_train);

    // for (auto& d : corpus.documents)
    //     for (auto& [word, freq] : d.tf_idf)
    //         cout << word << ": " << freq << endl; 

    return 0;
}