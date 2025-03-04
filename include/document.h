#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <atomic>
#include <cmath>
#include <cstdarg>
#include <sstream>
#include <fstream>
#include "categories.h"
#include "utils.h"
#include "Printable.h"

using namespace std;

class Category; // forward declaration


// class for single document
class Document : public Printable {

    public:

        int document_id{0};                           // id for testing purposes
        string text;                                  // overall text of document
        unordered_map<string, int> term_count;        // count of terms in document
        unordered_map<string, double> term_frequency; // frequency of terms in document
        unordered_map<string, double> tf_idf;         // tfidf of all terms in document
        int total_terms{0};                           // total number of terms in document
        text_cat_types_ category;                     // classification category ** result **

        // returns true if term exists in document
        bool is_term(string str);

        /* calculate term frequency for all terms in 
         * doc and insert into 'term_frequency'
         */
        void calculate_term_frequency_doc();

        /* -- Print Function -- */
        void print_all_info() const override {
            ofstream file{"test-output/lengthy/document-info.txt"};

            if (!file) {
                throw runtime_error("File Error in print_all_info");
                return;
            }

            file << "Info for Document id: " << document_id << "\n";
            file << print_category();
            file << print_number_terms();
            file << print_tf_idf();
            file << print_number_terms();
            
            file.close();
        }

    private:

        // return term frequency associated with a word in a doc
        double calculate_term_frequency(string term);

        /* private helper functions for printing */
        string print_text() const;
        string print_number_terms() const;
        string print_category() const;
        string print_tf_idf() const;
        string print_doc_term_count() const;
};

// class for entire corpus (collection of documents)
class Corpus : public Printable {

    public:

        vector<Document> documents;                               // vector of documents
        unordered_map<string, double> inverse_document_frequency; // tfidf of terms in corpus
        // unordered_map<string, int> document_frequency;         // # of documents each word appears in
        atomic<int> num_of_docs{0};                               // total number of documents

        /* calculate idf for each term in each document
         * and save it to document.tfidf (parallel)
         */
        void tfidf_documents();

        /* calculate idf for each term in each document
         * and save it to document.tfidf (sequential)
         */
        void tfidf_documents_seq();

        /* calculate idf for each term in each document
         * and save it to document.tfidf (parallel).
         * Uses ONE active thread per Document.
         */
        void tfidf_documents_not_dynamic();
        
        // returns number of unique terms in Corpus
        int get_num_unique_terms();

        /* -- Print Function -- */
        void print_all_info() const override {
            fstream file{"test-output/lengthy/corpus-info.txt"};

            if (!file) {
                throw runtime_error("File Error in print_all_info");
                return;
            }

            file << "Info for Corpus: \n";
            file << print_number_documents();
            file << print_number_threads_used();
            file << print_number_documents_per_thread();
            file << print_tfidf_entire();

            file.close();
        }

    private: 
        unsigned num_doc_per_thread;

        // return # of documents with term
        int num_doc_term(string str);

        // return inverse document frequency calculation of a certain term
        double idf_corpus(int docs_with_term);

        // using a thread insert tfidf into document. one thread per document
        void emplace_tfidf_document(Document * document);

        /* returns the number of Document objects that should
         * be computed in one thread.
         */
        unsigned get_number_of_docs_per_thread();

        /* private helper functions for printing */
        string print_number_threads_used() const;
        string print_number_documents_per_thread() const;
        string print_number_documents() const;
        string print_tfidf_entire() const;
};


#endif