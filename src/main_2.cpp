/* Test 2
 * 
 */

#include <chrono>
#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include "file_operations.h"
#include "tfidf.h"
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

    // print_a_vectored(corpus.documents[0].tf_idf);

    // initalize important terms for categories
    vector<Category> categories(MAX_CATEGORIES);
    get_all_category_important_terms(ref(categories), &corpus);

    for (int i = 0; i < MAX_CATEGORIES; i++) {
        cout << categories[i].category << endl;
        // cout << categories[0].most_important_terms[i].first << ": " << categories[0].most_important_terms[i].second << endl;
    }

    
    

    return 0;
}