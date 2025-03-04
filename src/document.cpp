/* document.cpp
 * source file for document.h
 */

#include "document.h"
#include "categories.h"
#include <fstream>

bool Document::is_term(string str) {
    return term_count.find(str) != term_count.end();
}

double Document::calculate_term_frequency(string term) {
    if (total_terms == 0) 
        return 0.0;

    return static_cast<double>(term_count[term]) / total_terms;
}

void Document::calculate_term_frequency_doc() {
    for (auto& [word, count] : term_count) 
        term_frequency[word] = calculate_term_frequency(word);
}

// /* -- Print Functions -- */
// void Document::print_all_info() {
//     ofstream file{"test-output/lengthy/document-info.txt"};
// 
//     if (!file) {
//         throw runtime_error("File Error in print_all_info");
//         return;
//     }
// 
//     file << "Info for Document id: " << document_id << "\n";
//     file << print_category();
//     file << print_number_terms();
//     file << print_tf_idf();
//     file << print_number_terms();
// 
// }
string Document::print_text() const {
    return "Text: " + text + "\n";
}
string Document::print_number_terms() const {
    return "Number of Terms: " + to_string(total_terms) + "\n";
}
string Document::print_category() const { 
    return "Category: " + text_categories.at(category) + "\n\n";
}
string Document::print_tf_idf() const {
    string tf_idf_str{"TF-IDF Vectorization: \n"};

    int i = 0;
    for (const auto& pair : tf_idf) {
        if (i % 5 == 0 && i != 0)
            tf_idf_str += "\n";
        else if (i != 0)
            tf_idf_str += "\t";
        tf_idf_str += pair.first + ": " + to_string(pair.second) + "\n";
    }
    tf_idf_str += "\n";

    return tf_idf_str;
}
string Document::print_doc_term_count() const {
    string term_count_str{"Number of Times Term Appeared in Document: \n"};
    
    int i = 0;
    for (const auto& pair : term_count) {
        if (i % 5 == 0 && i != 0)
            term_count_str += "\n";
        else if (i != 0)
            term_count_str += "\t";
        term_count_str += pair.first +" :" + to_string(pair.second);
        i++;
    }
    term_count_str += "\n";

    return term_count_str;
}
/* -- Print Functions -- */

double Corpus::idf_corpus(int docs_with_term) {
    return log(static_cast<double>(num_of_docs) / static_cast<double>(docs_with_term));
}

int Corpus::num_doc_term(string str) {
    int count{0};

    for (auto& d : documents) 
        if (d.is_term(str))
            count++;

    return count;
}

void Corpus::tfidf_documents() {
    vector<thread> threads;
    threads.reserve(NUMBER_OF_THREADS_MAX);

    unsigned number_of_docs_in_thread = get_number_of_docs_per_thread();
    num_doc_per_thread = number_of_docs_in_thread;
    unsigned number_of_docs_in_last_thread = num_of_docs % number_of_docs_in_thread;

    /* every 10 documents gets their own thread!!
     * done to reduce contention and reduce resource waste.
     */
    for (int i = 0; i < num_of_docs; i+=number_of_docs_in_thread) {
        threads.emplace_back([this, i, number_of_docs_in_thread, number_of_docs_in_last_thread]() {
            if (i == num_of_docs - number_of_docs_in_thread && number_of_docs_in_last_thread > 0) {
                for (unsigned x = 0; x < number_of_docs_in_last_thread; x++) 
                    emplace_tfidf_document(&documents[x+i]);
            } else {
                for (unsigned x = 0; x < number_of_docs_in_thread; x++) 
                    emplace_tfidf_document(&documents[x+i]);
            }
        });
    }

    for (auto& t : threads)
        t.join();
}

void Corpus::tfidf_documents_not_dynamic() {
    vector<thread> threads;

    /* every 10 documents gets their own thread!!
     * done to reduce contention and reduce resource waste.
     */
    for (int i = 0; i < num_of_docs; i+=10) {
        threads.emplace_back([this, i]() {
            for (int x = 0; x < 10; x++) 
                emplace_tfidf_document(&documents[x+i]);
        });
    }
}

// sequential
void Corpus::tfidf_documents_seq() {
    for (auto& document : documents) 
        emplace_tfidf_document(&document);
}

// using a thread insert tfidf into document. 
void Corpus::emplace_tfidf_document(Document * document) {
    for (const auto& [word, freq] : document->term_frequency)
        document->tf_idf[word] = freq * idf_corpus(num_doc_term(word));
}

int Corpus::get_num_unique_terms() {
    return inverse_document_frequency.size();
}

unsigned Corpus::get_number_of_docs_per_thread() {
    if (num_of_docs < NUMBER_OF_THREADS_MAX) 
        num_of_docs;

    return static_cast<unsigned>(num_of_docs) / NUMBER_OF_THREADS_MAX;
}

/* -- Print Functions -- */
string Corpus::print_number_threads_used() const {
    return "# Threads Used: " + to_string(NUMBER_OF_THREADS_MAX) + "\n";
}

string Corpus::print_number_documents_per_thread() const {
    return "# of Documents Sent to a Single Thread: " + to_string(num_doc_per_thread) + "\n";
}

string Corpus::print_number_documents() const {
    return "# of Documents: " + to_string(num_of_docs) + "\n";
}

string Corpus::print_tfidf_entire() const {
    string tfidf_corp_str{"\nEntire TF-IDF (Term: TF-IDF): \n"};
    int i{0};

    for (const auto& [word, count] : inverse_document_frequency) {
        if (i % 5 == 0)
            tfidf_corp_str += "\n";
        else 
            tfidf_corp_str += "\t";
        tfidf_corp_str += to_string(i) + ". " + word + ": " + to_string(count); 
        i++;
    }   

    if (i % 5 != 0)
        tfidf_corp_str += "\n";
    
    return tfidf_corp_str;
}