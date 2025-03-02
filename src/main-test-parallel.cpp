/* main-test-parrallel.cpp
 * 
 * Contains main for testing parallel tfidf with 
 * command line input. Command Line input should be
 * a '.csv' file in the following format:
 * --------------------
 * category,text
 * category.x,text.x 
 * --------------------
 * 
 * NOTE that you do not need the labels: 'category, text'
 * at the top of the file. HOWEVER, you must not have a
 * space between 'category,text' and/or 'category.x,text.x'
 */

#include <thread>
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
        vectorize_corpus_threaded(&corpus);
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    auto end = chrono::high_resolution_clock::now();
    
    print_duration_code(start, end, vectorization_);


    // calculate tfidf for every text in corpus
    start = chrono::high_resolution_clock::now();
    try {
        corpus.tfidf_documents();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    end = chrono::high_resolution_clock::now();
    // elapsed_time = elapsed_time_ms(start, end);//chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // cout << "TF-IDF Time: ";
    // print_elapsed_time_ms(start, end);
    print_duration_code(start, end, tfidf_);


    // get most important terms of category
    start = chrono::high_resolution_clock::now();
    vector<thread> cat_threads;
    vector<Category> cat_vect;

    try {
        for (int i = 0; i < 5; i++) {
            cat_threads.emplace_back(get_single_cat_par, &corpus, ref(cat_vect), i);
        }
    } catch (exception e) {
        cerr << "Error in get_single_cat_par: " << e.what() << endl;
        exit(1);
    }

    for (auto& cat : cat_threads)
        cat.join();
    
    end = chrono::high_resolution_clock::now();
    // elapsed_time = elapsed_time_ms(start, end);//chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // cout << "Categories Time: ";
    // print_elapsed_time_ms(start, end);
    print_duration_code(start, end, categories_);

    return 0;
}