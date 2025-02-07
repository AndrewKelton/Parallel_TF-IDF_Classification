#include "file_operations.h"

extern void read_csv_to_corpus(Corpus& corpus, const string& file_name) {
    ifstream file(file_name);

    if (!file.is_open()) {
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        Document new_document;
        new_document.text = line;
        corpus.documents.push_back(new_document);
    }   

    file.close();
}    

extern void write_to_csv_tfidf_corpus(Corpus corpus, const string& file_name) {
    
}