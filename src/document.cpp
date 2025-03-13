/* document.cpp
 * source file for document.h
 */

#include "document.hpp"
#include "categories.hpp"
#include <fstream>

namespace docs {

    bool Document::is_term(std::string str) {
        return term_count.find(str) != term_count.end();
    }

    double Document::calculate_term_frequency(std::string term) {
        if (total_terms == 0) 
            return 0.0;

        return static_cast<double>(term_count[term]) / total_terms;
    }

    void Document::calculate_term_frequency_doc() {
        for (auto& [word, count] : term_count) 
            term_frequency[word] = calculate_term_frequency(word);
    }

    void Document::print_all_info() {
        std::ofstream file{DOC_FILENAME, std::ios::app};

        if (!file) {
            throw std::runtime_error("File Error in print_all_info");
            return;
        }

        file << "Info for Document id: " << document_id << "\n";
        file << print_category();
        file << print_number_terms();
        file << print_tf_idf();
        
        file.close();
    }

    std::string Document::print_text() const {
        return "Text: " + text + "\n";
    }
    std::string Document::print_number_terms() const {
        return "Number of Terms: " + std::to_string(total_terms) + "\n";
    }
    std::string Document::print_category() const { 
        return "Category: " + text_categories.at(category) + "\n\n";
    }
    std::string Document::print_tf_idf() const {
        std::string tf_idf_str{"TF-IDF Vectorization: \n"};

        int i = 0;
        for (const auto& pair : tf_idf) {
            if (i % 5 == 0 && i != 0)
                tf_idf_str += "\n";
            else if (i != 0)
                tf_idf_str += "\t";
            tf_idf_str += pair.first + ": " + std::to_string(pair.second) + "\n";
        }
        tf_idf_str += "\n";

        return tf_idf_str;
    }
    std::string Document::print_doc_term_count() const {
        std::string term_count_str{"Number of Times Term Appeared in Document: \n"};
        
        int i = 0;
        for (const auto& pair : term_count) {
            if (i % 5 == 0 && i != 0)
                term_count_str += "\n";
            else if (i != 0)
                term_count_str += "\t";
            term_count_str += pair.first +" :" + std::to_string(pair.second);
            i++;
        }
        term_count_str += "\n";

        return term_count_str;
    }
} // namespace docs


namespace corpus { 

    double Corpus::idf_corpus(int docs_with_term) {
        return log(static_cast<double>(num_of_docs) / static_cast<double>(docs_with_term));
    }

    int Corpus::num_doc_term(const std::string& str) {
        int count{0};

        for (auto& d : documents) 
            if (d.is_term(str))
                count++;

        return count;
    }

    void Corpus::tfidf_documents() {
        std::vector<std::thread> threads;
        threads.reserve(NUMBER_OF_THREADS_MAX);

        unsigned number_of_docs_in_thread = get_number_of_docs_per_thread();
        num_doc_per_thread = number_of_docs_in_thread;
        unsigned number_of_docs_in_last_thread = num_of_docs % number_of_docs_in_thread;

        /* every num_doc_per_thread documents gets their own thread!!
        * done to reduce contention and resource waste.
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
        std::vector<std::thread> threads;

        /* every 10 documents gets their own thread!!
        * done to reduce contention and resource waste.
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
    void Corpus::emplace_tfidf_document(docs::Document * document) {
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
    std::string Corpus::print_number_threads_used() const {
        return "# Threads Used: " + std::to_string(NUMBER_OF_THREADS_MAX) + "\n";
    }

    std::string Corpus::print_number_documents_per_thread() const {
        return "# of Documents Sent to a Single Thread: " + std::to_string(num_doc_per_thread) + "\n";
    }

    std::string Corpus::print_number_documents() const {
        return "# of Documents: " + to_string(num_of_docs) + "\n";
    }

    // print king
    void Corpus::print_all_info() {
        std::ofstream file{docs::COR_FILENAME};

        if (!file) {
            throw std::runtime_error("File Error in print_all_info");
            return;
        }

        file << "Info for Corpus: \n";
        file << print_number_documents();
        file << print_number_threads_used();
        file << print_number_documents_per_thread();

        file.close();
    }
} // corpus namespace