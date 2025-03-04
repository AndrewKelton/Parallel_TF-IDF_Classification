/* main-test-non-optimized-parallel.cpp
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


/* 
 * **** NOTE TO SELF **** * 
 * 
 * USE AN ATOMIC COUNTER TO COUNT 
 * HOW MANY THREADS ARE USED IN 
 * EACH SECTION !!!!
 * 
 */


#include "count_vectorization.h"
#include "file_operations.h"
#include "categories.h"

using namespace std;

int main(int argc, char * argv[]) {

    // ensure valid number of input
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
    // comp_or_solo = (argv[2] == "solo"); // save testing type for later
    
    Corpus corpus;
    try {
        read_csv_to_corpus(ref(corpus), argv[1]);
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
    }

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
        corpus.tfidf_documents_not_dynamic();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    end = chrono::high_resolution_clock::now();

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

    print_duration_code(start, end, categories_);


    // convert txt results to csv for python graphing
    try {
        convert_results_txt_to_csv(0, comp_or_solo);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}