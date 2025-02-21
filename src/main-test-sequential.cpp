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

    vectorize_corpus_sequential(&corpus);
    cout << "Done vectorizing" << endl;

    auto start = chrono::high_resolution_clock::now();
    corpus.tfidf_documents_seq();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end - start;

    cout << "Done creating tfidf, time: " << elapsed_time.count() << endl;

    start = chrono::high_resolution_clock::now();
    vector<Category> cat_vect;
    for (int i = 0; i < 5; i++) {
        get_single_cat_seq(&corpus, ref(cat_vect), i);
    }
    end = chrono::high_resolution_clock::now();

    elapsed_time = end - start;

    cout << "Done creating categories, time: " << elapsed_time.count() << endl;


    // for (auto& cat : cat_vect)
    //     cat.print_info();

    return 0;
}