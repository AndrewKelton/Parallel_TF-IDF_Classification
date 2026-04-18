/* Corpus.cpp
 * source file for Corpus.hpp
 */

#include "Corpus.hpp"

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

        if (num_doc_per_thread == 1) {
            num_threads_used = num_of_docs;
        } else {
            num_threads_used = NUMBER_OF_THREADS_MAX;
        }

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

    void Corpus::tfidf_documents(int num_threads) {
        std::vector<std::thread> threads;

        unsigned number_of_docs_in_thread = get_number_of_docs_per_thread(num_threads);
        num_doc_per_thread = number_of_docs_in_thread;
        unsigned number_of_docs_in_last_thread = num_of_docs % number_of_docs_in_thread + number_of_docs_in_thread;

        if (num_doc_per_thread == 1) {
            num_threads_used = num_of_docs;
        } else {
            num_threads_used = num_threads;
        }
        
        /* every num_doc_per_thread documents gets their own thread!!
         * done to reduce contention and resource waste.
         */
        for (int i = 0; i < num_of_docs; i+=number_of_docs_in_thread) {
            threads.emplace_back([this, i, number_of_docs_in_thread, number_of_docs_in_last_thread]() {
                if (i == num_of_docs - number_of_docs_in_thread && number_of_docs_in_last_thread > 0) {
                    for (unsigned x = 0; x < number_of_docs_in_last_thread; x++) 
                        if (x + i < num_of_docs)
                            emplace_tfidf_document(&documents[x+i]);
                } else {
                    for (unsigned x = 0; x < number_of_docs_in_thread; x++) 
                        if (x + i < num_of_docs)
                            emplace_tfidf_document(&documents[x+i]);
                }
            });
        }
        
        /* TEST */
        // for (int i = 0; i < num_of_docs; i+=number_of_docs_in_thread) {
        //     if (i == num_of_docs - number_of_docs_in_thread && number_of_docs_in_last_thread > 0) {
        //         threads.emplace_back([this, i, number_of_docs_in_last_thread] {
        //             for (unsigned x = 0; x < number_of_docs_in_last_thread; x++) 
        //                 if (x + i < num_of_docs)
        //                     emplace_tfidf_document(&documents[x+i]);
        //         });
        //     } else {
        //         threads.emplace_back([this, i, number_of_docs_in_thread] {
        //             for (unsigned x = 0; x < number_of_docs_in_thread; x++) 
        //                 if (x + i < num_of_docs)
        //                     emplace_tfidf_document(&documents[x+i]);
        //         });
        //     }
        // }

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

    int Corpus::get_num_unique_terms() const {
        return inverse_document_frequency.size();
    }

    unsigned Corpus::get_number_of_docs_per_thread() const {
        if (num_of_docs < NUMBER_OF_THREADS_MAX) 
            num_of_docs;

        return static_cast<unsigned>(num_of_docs) / NUMBER_OF_THREADS_MAX;
    }

    unsigned Corpus::get_number_of_docs_per_thread(int num_of_threads) const {
        if (num_of_docs <= num_of_threads)
            return 1;
        return  static_cast<unsigned>(num_of_docs) / num_of_threads;
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
            std::cerr << "Failed to open file: " << docs::COR_FILENAME << " Error: " << strerror(errno) << std::endl;
            throw std::runtime_error("File Error in Document::print_all_info");
            return;
        }

        file << "Info for Corpus: \n";
        file << print_number_documents();
        file << print_number_threads_used();
        file << print_number_documents_per_thread();

        file.close();
    }
} // corpus namespace