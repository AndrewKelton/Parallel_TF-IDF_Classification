/* Test 2
 * 
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

    vectorize_corpus_threaded(&corpus);
    cout << "Done vectorizing" << endl;

    auto start = chrono::high_resolution_clock::now();
    corpus.tfidf_documents();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end - start;

    cout << "Done creating tfidf, time: " << elapsed_time.count() << endl;


    start = chrono::high_resolution_clock::now();
    vector<thread> cat_threads;
    vector<Category> cat_vect;
    for (int i = 0; i < 5; i++) {
        cat_threads.emplace_back(get_single_cat, &corpus, ref(cat_vect), i);
    }

    for (auto& cat : cat_threads)
        cat.join();
    
        end = chrono::high_resolution_clock::now();

    elapsed_time = end - start;

    cout << "Done creating categories, time: " << elapsed_time.count() << endl;

    // for (auto& cat : cat_vect)
    //     cat.print_info();

    return 0;
}