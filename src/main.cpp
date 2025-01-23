#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"

int main() {

    Document doc;
    doc.text = "Hello, word Hello";

    Document doc1;
    doc1.text = "Hello, world and hello car!";

    Document doc2;
    doc2.text = "That car is quite fast!";

    Document doc3;
    doc3.text = "That car is quite fast!";

    Corpus corpus;
    corpus.documents = {doc, doc1, doc2, doc3};

    vectorize_corpus_threaded(&corpus);
    corpus.tfidf_documents();

    for (auto& d : corpus.documents)
        for (auto& [word, freq] : d.tf_idf)
            cout << word << ": " << freq << endl;

    // string st{"hello"};    
    // cout << corpus.num_doc_term(st) << endl;
    // cout << corpus.documents[1].term_frequency[st] * corpus.idf_corpus(corpus.num_doc_term(st)) << endl;

    return 0;
}