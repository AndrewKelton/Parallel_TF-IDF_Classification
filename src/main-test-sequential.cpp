/* Test 2
 * 
 */

#include <iostream>
#include <chrono>
#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include "file_operations.h"
#include "categories.h"

using namespace std;

int main(int argc, char * argv[]) {

    if (argc <= 1) {
        cerr << "no files inputted..." << endl;
        return 1;
    }

    Corpus corpus;
    read_csv_to_corpus(ref(corpus), argv[1]);

    // vectorize corpus of documents
    auto start = chrono::high_resolution_clock::now();
    try {
        vectorize_corpus_sequential(&corpus);
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    auto end = chrono::high_resolution_clock::now();

    print_duration_code(start, end, vectorization_);


    // calculate tfidf for every text in corpus
    start = chrono::high_resolution_clock::now();
    try {
        corpus.tfidf_documents_seq();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    end = chrono::high_resolution_clock::now();

    print_duration_code(start, end, tfidf_);

    
    // get most important terms of category
    start = chrono::high_resolution_clock::now();
    vector<Category> cat_vect;
    try {
        for (int i = 0; i < 5; i++) 
            get_single_cat_seq(&corpus, ref(cat_vect), i);
        
    } catch (exception e) {
        cerr << "Error in get_single_cat_seq: " << e.what() << endl;
        exit(1);
    }
    end = chrono::high_resolution_clock::now();

    print_duration_code(start, end, categories_);

    return 0;
}