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
 * NOTE that you do not need the labels: 'category,text'
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


#include <iostream>
#include <unordered_map>
#include <cmath>
#include "count_vectorization.h"
#include "file_operations.h"
#include "flag_handler.hpp"

using namespace std;

// Function to compute cosine similarity between two TF-IDF vectors
double cosineSimilarity(const unordered_map<string, double>& doc1, const unordered_map<string, double>& doc2) {
    double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0;

    for (const auto& [word, tfidf1] : doc1) {
        // cout << "word: " << word << endl;
        if (doc2.find(word) != doc2.end()) {
            dotProduct += tfidf1 * doc2.at(word);
        }
        norm1 += tfidf1 * tfidf1;
    }
    
    for (const auto& [word, tfidf2] : doc2) {
        // cout << word << endl;
        norm2 += tfidf2 * tfidf2;
    }
    cout << "Norm1: " << norm1 << "\tNorm2: " << norm2 << "\tdotProduct: " << dotProduct << endl;

    if (fabs(norm1) < 1e-9 || fabs(norm2) < 1e-9) return 0.0; // Avoid division by zero

    return dotProduct / (sqrt(norm1) * sqrt(norm2));
}

string classifyText(const unordered_map<string, double>& unknownText, vector<Category> cat_vect) {
    text_cat_types_ best_category_type{invalid_t_};
    double maxSimilarity = 0.0;

    int i{0};
    for (const auto& cat_tf_idf : cat_vect) {
        double similarity = cosineSimilarity(unknownText, cat_tf_idf.tf_idf_all);
        if (similarity > maxSimilarity) {
            maxSimilarity = similarity;
            best_category_type = conv_cat_type(i);
        }
        i++;
    }

    return conv_cat_type(best_category_type);
}


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

    // cout << "HERE" << endl;

    /* initialize corpus and documents in 
     * corpus, from unkown text.
     */
    Corpus unknown_corpus;
    try {
        read_unkown_text(ref(unknown_corpus), "data/unkown_text.txt");
        cout << "HERE" << endl;
    } catch (runtime_error e) {
        cerr << "Error: " << argv[1] << " " << e.what() << endl;
    }
    
    try {
        vectorize_corpus_threaded(&unknown_corpus);
        unknown_corpus.num_of_docs += corpus.num_of_docs;
        cout << "HERE: " << unknown_corpus.num_of_docs << endl;
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return 1;
    }

    try {
        unknown_corpus.tfidf_documents_seq();
        cout << "HERE" << endl;
    } catch (exception e) {
        cerr << "Error in vectorize_corpus_threaded: " << e.what() << endl;
        return 1;
    }
    cout << "HERE" << endl;

    cout << "Unkown Text Classified: " << classifyText(unknown_corpus.documents[0].tf_idf, cat_vect) << endl;

    // check the user flags for output related tasks
    handle_output_flags(corpus, corpus.documents, cat_vect);

     // convert txt results to csv for python graphing
    // try {
    //     convert_results_txt_to_csv(0, comp_or_solo);
    // } catch (runtime_error e) {
    //     cerr << "Error converting txt to csv: " << e.what() << endl;
    //     exit(EXIT_FAILURE);
    // }

    return 0;
}