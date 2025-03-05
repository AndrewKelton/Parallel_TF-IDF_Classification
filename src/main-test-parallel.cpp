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
    Corpus corpus;
    try {
        read_csv_to_corpus(ref(corpus), argv[1]);
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
    }
    
    
    /* -- Vectorize Documents Section -- */
    auto start = chrono::high_resolution_clock::now();

    try {
        vectorize_corpus_threaded(&corpus);
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, vectorization_);
    /* -- Vectorize Documents Section END -- */


    /* -- Calculate TF-IDF Section -- */
    start = chrono::high_resolution_clock::now();

    try {
        corpus.tfidf_documents();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return 1;
    }

    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, tfidf_);
    /* -- Calculate TF-IDF Section END -- */


    /* -- Category Section -- */
    start = chrono::high_resolution_clock::now();
    vector<thread> cat_threads;
    vector<Category> cat_vect;

    try {
        for (int i = 0; i < 5; i++) {
            cat_threads.emplace_back(get_single_cat_par, &corpus, ref(cat_vect), conv_cat_type(i));
        }
    } catch (exception e) {
        cerr << "Error in get_single_cat_par: " << e.what() << endl;
        return 1;
    }

    for (auto& cat : cat_threads)
        cat.join();
    
    end = chrono::high_resolution_clock::now();

    print_duration_code(start, end, categories_);
    /* -- Category Section END -- */


    // convert txt results to csv for python graphing
    try {
        convert_results_txt_to_csv(0, comp_or_solo);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    // check the user flags for output related tasks
    handle_output_flags(corpus, corpus.documents, cat_vect);

    return 0;
}