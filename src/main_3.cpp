#include <chrono>
#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include "file_operations.h"

using namespace std;

int main(int argc, char * argv[]) {

    Corpus corpus;
    read_csv_to_corpus(ref(corpus), argv[1]);
    // cout << "read file, # of docs: " << corpus.num_of_docs << endl;
    vectorize_corpus_threaded(&corpus);
    cout << "Done vectorizing" << endl;

    auto start = chrono::high_resolution_clock::now();
    corpus.tfidf_documents();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end - start;
    cout << "Done creating tfidf, time: " << elapsed_time.count() << endl;

    for (auto& d : corpus.documents)
        for (auto& [word, freq] : d.tf_idf)
            cout << word << ": " << freq << endl; 

    return 0;
}