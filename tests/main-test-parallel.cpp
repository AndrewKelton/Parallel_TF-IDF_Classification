/**
 * @defgroup UsageTestFiles Usage Test Files
 * @brief Group containing all test files, showing project's usage.
 * @{
 */

/**
 * @file main-test-parallel.cpp
 * @brief Main program for testing parallel TF-IDF computation with command-line input.
 * @ingroup UsageTestFiles
 * 
 * @details This file contains the main function that performs parallel TF-IDF computation. 
 * The program accepts a command-line input in the form of a `.csv` file, where each line 
 * contains a document's category and text.
 * 
 * The expected format for the input `.csv` file is as follows:
 * 
 * --------------------
 * category,text
 * category.x,text.x
 * --------------------
 * 
 * @note The input file should not contain the labels "category,text" at the top. 
 * Additionally, there must not be any spaces between "category,text" and "category.x,text.x".
 */

/* 
 * **** NOTE TO SELF **** * 
 * 
 * USE AN ATOMIC COUNTER TO COUNT 
 * HOW MANY THREADS ARE USED IN 
 * EACH SECTION !!!!
 * 
 */


// #include <iostream>
// #include <unordered_map>
// #include <cmath>
#include "count_vectorization.hpp"
#include "file_operations.hpp"
#include "flag_handler.hpp"

using namespace std;

int main(int argc, char * argv[]) {

    // ensure valid number of input
    if (argc <= 2) {
        cerr << "not enough inputs..." << endl;
        return EXIT_FAILURE;
    }

    /* Used for determining which directory (comparison or solo)
     * we are testing inside of. Used for converting plaintext file
     * to csv file after data has been processed 
     */
    string arg_2 = argv[2];

    // ensure valid testing type    
    if (arg_2 != "solo" && arg_2 != "comparison") {
        cerr << "invalid second parameter... usage: 'solo' or 'comparison'" << endl;
        return EXIT_FAILURE;
    } 

    bool comp_or_solo{(arg_2 == "solo")}; 
    
    /* initialize corpus and documents in 
     * corpus, while reading input from csv.
     */
    corpus::Corpus corpus;
    try {
        read_csv_to_corpus(ref(corpus), argv[1]);
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
        return EXIT_FAILURE;
    }
    
    
    /* -- Vectorize Documents Section -- */
    auto start = chrono::high_resolution_clock::now();

    try {
        vectorize_corpus_threaded(&corpus);
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    auto end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, vectorization_);
    /* -- Vectorize Documents Section END -- */


    /* -- Calculate TF-IDF Section -- */
    start = chrono::high_resolution_clock::now();

    try {
        corpus.tfidf_documents();
    } catch (exception e) {
        cerr << "Error in corpus.tfidf_documents(): " << e.what() << endl;
        return EXIT_FAILURE;
    }

    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, tfidf_);
    /* -- Calculate TF-IDF Section END -- */


    /* -- Category Section -- */
    start = chrono::high_resolution_clock::now();
    vector<thread> cat_threads;
    vector<cats::Category> cat_vect;

    try {
        for (int i = 0; i < 5; i++) {
            cat_threads.emplace_back([&, i]() {
                cats::par::get_single_cat_par(corpus, ref(cat_vect), conv_cat_type(i));
            });
        }
    } catch (exception e) {
        cerr << "Error in get_single_cat_par: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    for (auto& cat : cat_threads)
        cat.join();
    
    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, categories_);
    /* -- Category Section END -- */


    /* initialize corpus and documents in 
     * corpus, from unknown text.
     */
    start = chrono::high_resolution_clock::now();

    corpus::Corpus unknown_corpus;
    try {
        read_unknown_text(ref(unknown_corpus), "data/unknown_text.txt");
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
        return EXIT_FAILURE;
    }
    
    try {
        vectorize_corpus_threaded(&unknown_corpus);
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    try {
        unknown_corpus.tfidf_documents();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    vector<string> unknown_cats_correct = read_unknown_cats();

    // classify unknown documents
    cats::par::init_classification_par(ref(unknown_corpus), ref(cat_vect), unknown_cats_correct);
    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, unknown_);
    cats::print_classifications();
    // cats::print_classifications(cats::init_classification_par(ref(unknown_corpus), ref(cat_vect), unknown_cats_correct));

    // convert txt results to csv for python graphing
    try {
        convert_results_txt_to_csv(0, comp_or_solo);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    // check the user flags for output related tasks
    handle_output_flags(corpus, corpus.documents, cat_vect);

    return 0;
}

/** @} */ // End of UsageTestFiles group