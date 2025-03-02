/* main-test-sequential.cpp
 * 
 * Contains main for testing sequential tfidf with 
 * command line input. Command Line input should be
 * a '.csv' file in the following format:
 * --------------------
 * category,text
 * category.x,text.x 
 * --------------------
 * 
 * NOTE that you do not need the labels: 'category,text'
 * at the top of the file. HOWEVER, you must not have a
 * space between 'category,text' and/or 'category.x,text.x'
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

    if (argc <= 2) {
        cerr << "not enough inputs..." << endl;
        exit(EXIT_FAILURE);
    }

    /* Used for determining which directory (comparison or solo)
     * we are testing inside of. Used for converting plaintext file
     * to csv file after data has been processed 
     */
    string arg_2 = argv[2];

    // ensure valid testing type
    if (arg_2 != "solo" && arg_2 != "comparison") {
        cerr << "invalid second parameter... usage: 'solo' or 'comparison'" << endl;
        exit(EXIT_FAILURE);
    } 

    bool comp_or_solo{(arg_2 == "solo") /* false == comparison, true == solo */}; 

    Corpus corpus;
    try {
        read_csv_to_corpus(ref(corpus), argv[1]);
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
    }


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


    // convert txt results to csv for python graphing
    try {
        convert_results_txt_to_csv(1, comp_or_solo);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}