#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include <thread>

// not actually final
void do_all(Document * doc) {
    preprocess_text(doc);
    count_words_doc(doc);
    (*doc).calculate_term_frequency_doc();
}

int main() {

    vector<thread> threads;

    Document doc;
    doc.text = "Hello, word Hello";

    Document doc1;
    doc1.text = "Hello, world and hello cars!";

    Document doc2;
    doc2.text = "That car is quite fast!";

    Corpus corpus;
    corpus.documents = {doc, doc1, doc2};
    corpus.num_of_docs = 3;

    for (auto& d : corpus.documents)
        threads.emplace_back(thread(do_all, &d));
    
    for (auto& t : threads)
        t.join();

    // for (auto& [word, count] : corpus.documents[1].term_count)
    //     cout << word << ": " << count << endl;

    corpus.idf_documents();

    for (auto& d : corpus.documents) {
        for (auto& [word, freq] : d.tf_idf)
            cout << word << ": " << freq << endl;
    }


    // string st{"hello"};    
    // cout << corpus.num_doc_term(st) << endl;
    // cout << corpus.documents[1].term_frequency[st] * corpus.idf_corpus(corpus.num_doc_term(st)) << endl;

    return 0;
}