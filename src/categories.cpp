/* categories.cpp
 * source file for categories.hpp
 */

#include "categories.hpp"
#include "document.hpp"
#include "utils.hpp"
#include <mutex>
#include <algorithm>
#include <exception>
#include <sstream>
#include <fstream>

std::mutex mtx; // global mtx for emplacing Category to thread
std::mutex tf_idf_mutex;

namespace cats { // namespace cats
    unknown_classification_s u_classified = Unknown_Classification_Corp_S();
    
    // return sorted std::vector of tfidf terms
    std::vector<std::pair<std::string, double>> Category::sort_unordered_umap(std::unordered_map<std::string, double> terms) {
        if (terms.empty())
            throw_runtime_error("no terms or terms are empty in ", conv_cat_type(this->category_type));

        std::vector<std::pair<std::string, double>> vectored_umap(terms.begin(), terms.end());

        std::sort(vectored_umap.begin(), vectored_umap.end(), [](const auto&a, const auto&b) {
            return a.second > b.second;
        });

        return vectored_umap;
    }

    // return std::pair for nth important tfidf term in category
    std::pair<std::string, double> Category::search_nth_important_term(std::vector<std::vector<std::pair<std::string, double>>> all_tfidf_terms, std::vector<std::pair<std::string, double>> used) {

        if (all_tfidf_terms.empty()){
            throw_runtime_error("empty tfidf in ", conv_cat_type(this->category_type));
        }
        if (all_tfidf_terms[0].empty()) {
            throw_runtime_error("empty tfidf in ", conv_cat_type(this->category_type));
        }
        
        std::pair<std::string, double> current_high = all_tfidf_terms[0][0];

        for (auto& row : all_tfidf_terms) {

            /* only checking first 5 terms in a row, 
            * since only need 5 important terms
            */
            for (int i = 0; i < std::min(5, static_cast<int>(row.size())); i++) {
                std::pair<std::string, double> current_pair = row[i];

                if ((current_high.second < current_pair.second && find(used.begin(), used.end(), current_pair) == used.end()) || find(used.begin(), used.end(), current_high) != used.end())
                    current_high = current_pair;
            }
        }

        return current_high;
    }

    void Category::print_all() const {
        for (auto& [term, tf_idf] : this->tf_idf_all) {
            std::cout << term << ": " << tf_idf << std::endl;
        }
    }

    void Category::put_tf_idf_all(std::unordered_map<std::string, double> doc_tf_idf) {
        std::unordered_map<std::string, int> word_count;
        int i{0};

        // std::lock_guard<std::mutex> lock(tf_idf_mutex);  // Protects tf_idf_all

        for (auto& tf_idf : doc_tf_idf) {
            i++;
            // cout << tf_idf.first << " " << tf_idf.second << std::endl;
            auto founded = tf_idf_all.find(tf_idf.first);
            if (founded != tf_idf_all.end()) {
                double new_val = (tf_idf.second + founded->second);
                tf_idf_all[tf_idf.first] = new_val;
                word_count[tf_idf.first]++;
            } else {
                tf_idf_all[tf_idf.first] = tf_idf.second;
                word_count[tf_idf.first] = 1;
            }
        }

        for (auto& w_to_count : word_count) {
            // tf_idf_all
            tf_idf_all[w_to_count.first] = tf_idf_all[w_to_count.first] / i;
        }
    }


    void Category::get_important_terms(const corpus::Corpus& corpus) {
        this->most_important_terms.reserve(5);                                       // reserve 5 slots of memory
        std::vector<std::vector<std::pair<std::string, double>>> vectored_all_umaps; // std::vectorized sorted tfidf mapping
        
        // sort all the terms for each Document in the Category
        for (auto& document : corpus.documents) {
            if (document.category != category_type)
                continue;

            try {
                vectored_all_umaps.emplace_back(sort_unordered_umap(document.tf_idf));
                put_tf_idf_all(document.tf_idf);
            } catch (const std::runtime_error& e) {
                std::cerr << "RuntimeError in Category::sort_unordered_umap: " << e.what() << std::endl;
                throw std::runtime_error("RuntimeError in Category::get_important_terms");
            } catch (const std::exception& e) {
                std::cerr << "Exception in Category::sort_unordered_umap: " << e.what() << std::endl;
                throw std::runtime_error("Exception in Category::get_important_terms"); 
            } 
        }

        // get the 5 most important terms 
        for (int i = 0; i < 5; i++) {
            try {
                most_important_terms.emplace_back(search_nth_important_term(vectored_all_umaps, most_important_terms));
            } catch (const std::runtime_error& e) {
                std::cerr << "RuntimeError in Category::search_nth_important_term: " << e.what() << std::endl;
                throw std::runtime_error("RuntimeError in Category::get_important_terms");
            } catch (const std::exception& e) {
                std::cerr << "Exception in Category::search_nth_important_term: " << e.what() << std::endl;
                throw std::runtime_error("Exception in Category::get_important_terms"); 
            }
        }
    }

    void Category::print_all_info() const {
        std::ofstream file{cats::CAT_FILENAME, std::ios::app};

        if (!file) {
            throw std::runtime_error("File Error in print_all_info");
            return;
        }

        file << "Category: " << conv_cat_type(category_type) << "\n";
        
        for (auto& term : most_important_terms) {
            file << term.first << ": " << term.second << "\n";
        }
        file << "\n";
        file.close();
    }


    static double cosine_similarity(const std::unordered_map<std::string, double>& doc1, const std::unordered_map<std::string, double>& doc2) {
        double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0;

        for (const auto& [word, tfidf1] : doc1) {
            // cout << "word: " << word << std::endl;
            if (doc2.find(word) != doc2.end()) {
                dotProduct += tfidf1 * doc2.at(word);
            }
            norm1 += tfidf1 * tfidf1;
        }
        
        for (const auto& [word, tfidf2] : doc2) {
            // cout << word << std::endl;
            norm2 += tfidf2 * tfidf2;
        }
        // cout << "Norm1: " << norm1 << "\tNorm2: " << norm2 << "\tdotProduct: " << dotProduct << std::endl;

        if (fabs(norm1) < 1e-9 || fabs(norm2) < 1e-9) return 0.0; // Avoid division by zero

        return dotProduct / (sqrt(norm1) * sqrt(norm2));
    }

    extern unknown_class classify_text(const std::unordered_map<std::string, double>& unknownText, std::vector<Category> cat_vect, std::string correct_type) {
        unknown_class unknown_classification;
        unknown_classification.correct_type = conv_cat_type(correct_type);
        text_cat_types_ best_category_type{invalid_t_};
        double maxSimilarity = 0.0;

        int i{0};
        for (const auto& cat_tf_idf : cat_vect) {

            double similarity = cosine_similarity(unknownText, cat_tf_idf.tf_idf_all);
            if (similarity > maxSimilarity) {
                maxSimilarity = similarity;
                best_category_type = conv_cat_type(i);
                unknown_classification.classified_type = conv_cat_type(i);
            }
            i++;
        }

        // cout << correct_type << "\t" << conv_cat_type(unknown_classification.classified_type) << std::endl;
        if (unknown_classification.correct_type == unknown_classification.classified_type) {
            unknown_classification.correct = true;
            // cout << "True";
        } else {
            unknown_classification.correct = false;
            // cout << "False";
        }

        return unknown_classification;
    }

    extern void print_classifications() {
        std::cout << "# of total unknown Documents: " << u_classified.total_count << std::endl;
        std::cout << "# of total correctly Classfied: " << u_classified.correct_count << std::endl;
        std::cout << "% Classifed Correctly: " << u_classified.correct_db << "%" << std::endl;
        std::cout << "Actual\tClassified\tCorrect" << std::endl;
        
        if (u_classified.unknown_doc.empty()) {
            std::cerr << "No documents classified!" << std::endl;
        }

        for (auto doc : u_classified.unknown_doc) {
            std::cout << conv_cat_type(doc.correct_type) << "\t" << conv_cat_type(doc.classified_type) << "\t";
            
            if (doc.correct)
                std::cout << "True";
            else 
                std::cout << "False";
            
            std::cout << std::endl;
        }
    }

    // initialize categories std::vector
    static std::vector<Category> init_categories() {
        std::vector<Category> categories_list;

        for (int i = 0; i < MAX_CATEGORIES; i++)
            categories_list.emplace_back(i);
        
        return categories_list;
    }

    // deprecated function
    extern std::vector<Category> get_all_category_important_terms(const corpus::Corpus& corpus) {
        std::vector<Category> categories_list = init_categories();
        std::vector<std::thread> category_threads;

        for (int i = 0; i < MAX_CATEGORIES; i++) {
            std::cout << "getting important categories for " << conv_cat_type(conv_cat_type(i)) << std::endl;
            category_threads.emplace_back([&] {
                try {
                    categories_list[i].get_important_terms(corpus);
                } catch (const std::runtime_error &e) {
                    std::cerr << "RuntimeError get_all_category_important_terms: " << e.what() << std::endl;
                    return;
                } catch (const std::exception &e) {
                    std::cerr << "Exception get_all_category_important_terms: " << e.what() << std::endl;
                    return;
                }
            });
        }

        for (auto& thread : category_threads)
            thread.join();

        return categories_list;
    }
}

/* Parallel Functions */
namespace cats::par { // namespace cats::par
    std::atomic<int> correct_count{0};
    std::atomic<int> total_count{0};
    std::mutex c_mtx;

    extern void get_single_cat_par(const corpus::Corpus& corpus, std::vector<Category>& cats, text_cat_types_ catint) {
        std::lock_guard<std::mutex> lock(mtx); /* MOVING LOCK HERE INCREASED ACCURACY BY ABOUT 60% */
        Category cat(catint);

        try {
            cat.get_important_terms(corpus);
            cats.emplace_back(std::move(cat));
        } catch (const std::runtime_error &e) {
            std::cerr << "RuntimeError in get_single_cat_par: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            std::cerr << "Exception in get_single_cat_par: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        
      /*{
            std::lock_guard<std::mutex> lock(mtx); ** HAVING LOCK HERE DECREASED ACCURACY BY ABOUT 60% **
            cats.emplace_back(std::move(cat));
        }*/
    }

    extern std::vector<cats::Category> get_all_cat_par(const corpus::Corpus& corpus) {
        std::vector<std::thread> cat_threads;
        std::vector<cats::Category> cat_vect;

        try {
            for (int i = 0; i < 5; i++) {
                cat_threads.emplace_back([&, i]() {
                    cats::par::get_single_cat_par(corpus, ref(cat_vect), conv_cat_type(i));
                });
            }
        } catch (std::exception e) {
            std::cerr << "Error in get_single_cat_par: " << e.what() << std::endl;
        }

        for (auto& cat : cat_threads)
            cat.join();

        return cat_vect;
    }

    // commit classification changes to the unknown_classification_par_s structure
    static void commit_classification_changes(std::unordered_map<std::string, double> tf_idf, std::vector<Category> cat_vect, std::string correct_type) {
        try {    
            unknown_class result = classify_text(tf_idf, cat_vect, correct_type);
            if (result.correct)
                correct_count.fetch_add(1, std::memory_order_release);
            total_count.fetch_add(1, std::memory_order_release);

            std::lock_guard<std::mutex> lock(c_mtx);
            u_classified.unknown_doc.emplace_back(result);
        } catch (std::exception &e) {
            std::cerr << "Failure in commit_classification_changes: " << "Error: " << strerror(errno) << std::endl;
            return;
        }
    }
    
    extern void init_classification_par(const corpus::Corpus& unknown_corpus, std::vector<Category> cat_vect, std::vector<std::string> correct_types) {
        unsigned int number_of_docs_in_thread{unknown_corpus.get_number_of_docs_per_thread()};
        int num_of_docs{unknown_corpus.num_of_docs};
        unsigned number_of_docs_in_last_thread = num_of_docs % number_of_docs_in_thread;
        std::vector<std::thread> threads;

        // load balanced threads accordingly
        for (int i = 0; i < num_of_docs; i+=number_of_docs_in_thread) {
            threads.emplace_back([num_of_docs, i, number_of_docs_in_thread, number_of_docs_in_last_thread, &unknown_corpus, &cat_vect, &correct_types]() mutable {
                int x{0};
                try {
                    if (i == num_of_docs - number_of_docs_in_thread && number_of_docs_in_last_thread > 0) {
                        for ( x = 0; x < number_of_docs_in_last_thread; x++) 
                            try {
                                commit_classification_changes(unknown_corpus.documents.at(x+i).tf_idf, cat_vect, correct_types.at(x+i));
                            } catch (std::out_of_range &e) {
                                std::cerr << "Error: " << " in init_classification_par, i=" << i << ", x=" << x << std::endl << e.what() << std::endl;
                                exit(EXIT_FAILURE);
                            }
                    } else {
                        for ( x = 0; x < number_of_docs_in_thread; x++) {
                            try {
                                commit_classification_changes(unknown_corpus.documents.at(x+i).tf_idf, cat_vect, correct_types.at(x+i));
                            } catch (std::out_of_range &e) {
                                std::cerr << "Error: " << " in init_classification_par, i=" << i << ", x=" << x << std::endl << e.what() << std::endl;
                                exit(EXIT_FAILURE);
                            }
                        }
                    }   
                } catch (std::out_of_range &e) {
                    std::cerr << "Error: " << " in init_classification_par, i=" << i << ", x=" << x << std::endl << e.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
            });
        }

        for (auto& t : threads)
            t.join();

        u_classified.correct_count = correct_count.load();
        u_classified.total_count = total_count.load();

        u_classified.correct_db = static_cast<double>(u_classified.correct_count) / u_classified.total_count * 100;
    }
}

/* Sequential Functions */
namespace cats::seq { // namespace cats::seq

    extern void get_single_cat_seq(const corpus::Corpus& corpus, std::vector<Category>& cats, text_cat_types_ catint) {
        Category cat(catint);
        try {
            cat.get_important_terms(corpus);
        } catch (const std::runtime_error &e) {
            std::cerr << "RuntimeError in get_single_cat_seq: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        } catch (const std::exception &e) {
            std::cerr << "Exception in get_single_cat_seq: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        cats.emplace_back(std::move(cat));
    }

    extern std::vector<cats::Category> get_all_cat_seq(const corpus::Corpus&  corpus) {
        std::vector<cats::Category> cat_vect;
        try {
            for (int i = 0; i < 5; i++) 
                cats::seq::get_single_cat_seq(corpus, ref(cat_vect), conv_cat_type(i));
            
        } catch (std::exception e) {
            std::cerr << "Error in get_single_cat_seq: " << e.what() << std::endl;
        }

        return cat_vect;
    }

    extern void init_classification_seq(const corpus::Corpus& unknown_corpus, std::vector<Category> cat_vect, std::vector<std::string> correct_types) {
        u_classified.correct_count = 0; // ensure set to 0
        u_classified.total_count = 0;   // ensure set to 0
        int num_of_docs{static_cast<int>(unknown_corpus.documents.size())};

        for (int i = 0; i < num_of_docs; i++) {
            if (i >= unknown_corpus.documents.size() || i >= correct_types.size()) {
                std::cerr << "Index out of range: " << i << std::endl;
            } else {
                try {
                    auto doc = unknown_corpus.documents.at(i);
                    auto correct_type = correct_types.at(i);
                    auto result = classify_text(doc.tf_idf, cat_vect, correct_type);
            
                    u_classified.total_count++;
                    if (result.correct)
                        u_classified.correct_count++;

                    u_classified.unknown_doc.emplace_back(result);
                } catch (const std::out_of_range& e) {
                    std::cerr << "Caught exception: " << e.what() << std::endl;
                    std::cerr << "Index: " << i << ", unknown_corpus.documents.size(): " << unknown_corpus.documents.size() << ", correct_types.size(): " << correct_types.size() << std::endl;
                }
            }
        }

        u_classified.correct_db = static_cast<double>(u_classified.correct_count) / u_classified.total_count * 100;
    }
}