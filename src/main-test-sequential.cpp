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


#include "count_vectorization.h"
#include "file_operations.h"
#include "flag_handler.hpp"

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
        vectorize_corpus_sequential(&corpus);
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    auto end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, vectorization_);
    /* -- Vectorize Documents Section END -- */


    /* -- Calculate TF-IDF Section -- */
    start = chrono::high_resolution_clock::now();
    try {
        corpus.tfidf_documents_seq();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        exit(1);
    }
    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, tfidf_);
    /* -- Calculate TF-IDF Section END -- */

    
    /* -- Category Section -- */
    start = chrono::high_resolution_clock::now();
    vector<Category> cat_vect;
    try {
        for (int i = 0; i < 5; i++) 
            get_single_cat_seq(&corpus, ref(cat_vect), conv_cat_type(i));
        
    } catch (exception e) {
        cerr << "Error in get_single_cat_seq: " << e.what() << endl;
        exit(1);
    }
    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, categories_);
    /* -- Category Section END -- */


    /* initialize corpus and documents in 
     * corpus, from unkown text.
     */
    start = chrono::high_resolution_clock::now();
    
    Corpus unknown_corpus;
    try {
        read_unkown_text(ref(unknown_corpus), "data/unkown_text.txt");
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
    }
    
    try {
        vectorize_corpus_sequential(&unknown_corpus);
        // unknown_corpus.num_of_docs += corpus.num_of_docs;
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return 1;
    }

    try {
        unknown_corpus.tfidf_documents_seq();
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return 1;
    }

    vector<string> unknown_cats_correct = read_unkown_cats();

    print_classifications(init_classification(&unknown_corpus, cat_vect, unknown_cats_correct));
    end = chrono::high_resolution_clock::now();
    print_duration_code(start, end, unknown_);


    // convert txt results to csv for python graphing
    try {
        convert_results_txt_to_csv(1, comp_or_solo);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    // check the user flags for output related tasks
    handle_output_flags(corpus, corpus.documents, cat_vect);

    return 0;
}