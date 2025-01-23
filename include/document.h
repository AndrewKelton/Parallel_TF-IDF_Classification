#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// class for single document
class Document {
    public:
        string text;                                  // overall text of document
        unordered_map<string, int> term_count;        // count of terms in document
        unordered_map<string, double> term_frequency; // frequency of terms in document
        unordered_map<string, double> tf_idf;         // tfidf of all terms in document
        int total_terms = 0;                          // total number of terms in document

        // returns true if term exists in document
        bool is_term(string str);

        // return term frequency associated with a word in a doc
        double calculate_term_frequency(string term);

        /* calculate term frequency for all terms in 
         * doc and insert into 'term_frequency'
         */
        void calculate_term_frequency_doc();
        
        // other functions here

};

// class for entire corpus (collection of documents)
class Corpus {
    public:
        vector<Document> documents;                       // list of documents
        unordered_map<string, double> document_frequency; // frequency of terms in corpus
        int num_of_docs = 0;                              // total number of documents

        // return # of documents with term
        int num_doc_term(string str);

        // inverse document frequency calculation
        double idf_corpus(int docs_with_term);

        /* calculate idf for each term in each document
         * and save it to document.tfidf
         */
        void idf_documents();


        // other functions here
};

#endif