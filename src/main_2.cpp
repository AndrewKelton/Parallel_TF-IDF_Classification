/* Test 2
 * 
 */

#include <chrono>
#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include "file_operations.h"
// #include "tfidf.h"
#include "categories.h"
// #include <mlpack.hpp>
// #include <mlpack/methods/logistic_regression/logistic_regression.hpp>

using namespace std;

int main() {

    Corpus corpus;
    read_csv_to_corpus(ref(corpus), "data/bbc-text.csv");
    // cout << "read file, # of docs: " << corpus.num_of_docs << endl;
    vectorize_corpus_threaded(&corpus);
    cout << "Done vectorizing" << endl;

    auto start = chrono::high_resolution_clock::now();
    corpus.tfidf_documents();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end - start;
    cout << "Done creating tfidf, time: " << elapsed_time.count() << endl;

    // int x = 0;
    // for (auto& doc : corpus.documents) {
    //     if (doc.category == sport_t) {
    //         cout << "Document is sport" << endl;
    //         x++;
    //     }
    // }

    // cout << x << endl;

    vector<thread> cat_threads;
    vector<Category> cat_vect;
    for (int i = 0; i < 5; i++) {
        cat_threads.emplace_back(get_single_cat, &corpus, ref(cat_vect), i);
    }
    // for (auto& cat : cat_vect)
    //     cat.print_info();

    for (auto& cat : cat_threads)
        cat.join();

    for (auto& cat : cat_vect)
        cat.print_info();

//     vector<Category> categories = get_all_category_important_terms(&corpus);
// 
//     for (int i = 0; i < MAX_CATEGORIES; i++) {
//         categories[i].print_info();
//     }



    return 0;
}