/* categories.cpp
 * source file for categories.h
 */

#include "categories.h"
#include "document.h"
#include <mutex>
#include <algorithm>
#include <exception>
#include <sstream>
#include <fstream>

using namespace std;

mutex mtx; // global mtx for emplacing Category to thread

// return sorted vector of tfidf terms
vector<pair<string, double>> Category::sort_unordered_umap(unordered_map<string, double> terms) {
    if (terms.empty())
        throw_runtime_error("no terms or terms are empty in ", conv_cat_type(this->category_type));

    vector<pair<string, double>> vectored_umap(terms.begin(), terms.end());

    sort(vectored_umap.begin(), vectored_umap.end(), [](const auto&a, const auto&b) {
        return a.second > b.second;
    });

    return vectored_umap;
}

// return pair for nth important tfidf term in category
pair<string, double> Category::search_nth_important_term(vector<vector<pair<string, double>>> all_tfidf_terms, vector<pair<string, double>> used) {

    if (all_tfidf_terms.empty()){
        throw_runtime_error("empty tfidf in ", conv_cat_type(this->category_type));
    }
    if (all_tfidf_terms[0].empty()) {
        throw_runtime_error("empty tfidf in ", conv_cat_type(this->category_type));
    }
    
    pair<string, double> current_high = all_tfidf_terms[0][0];

    for (auto& row : all_tfidf_terms) {

        /* only checking first 5 terms in a row, 
         * since only need 5 important terms
         */
        for (int i = 0; i < min(5, static_cast<int>(row.size())); i++) {
            pair<string, double> current_pair = row[i];

            if ((current_high.second < current_pair.second && find(used.begin(), used.end(), current_pair) == used.end()) || find(used.begin(), used.end(), current_high) != used.end())
                current_high = current_pair;
        }
    }

    return current_high;
}

void Category::print_all() {
    for (auto& [term, tf_idf] : this->tf_idf_all) {
        cout << term << ": " << tf_idf << endl;
    }
}

void Category::put_tf_idf_all(unordered_map<string, double> doc_tf_idf) {
    unordered_map<string, int> word_count;
    int i{0};
    for (auto& tf_idf : doc_tf_idf) {
        i++;
        // cout << tf_idf.first << " " << tf_idf.second << endl;
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


void Category::get_important_terms(Corpus * corpus) {

    this->most_important_terms.reserve(5);                   // reserve 5 slots of memory
    vector<vector<pair<string, double>>> vectored_all_umaps; // vectorized sorted tfidf mapping
    
    // sort all the terms for each Document in the Category
    for (auto& document : corpus->documents) {
        if (document.category != category_type)
            continue;

        try {
            vectored_all_umaps.emplace_back(sort_unordered_umap(document.tf_idf));
            put_tf_idf_all(document.tf_idf);
        } catch (const runtime_error& e) {
            cerr << "Error Category::get_important_terms: " << e.what() << endl;
            break;
        } catch (const exception& e) {
            cerr << "Error Category::get_important_terms: " << e.what() << endl;
            break;
        } 
    }

    // get the 5 most important terms 
    for (int i = 0; i < 5; i++) {
        try {
            most_important_terms.emplace_back(search_nth_important_term(vectored_all_umaps, most_important_terms));
        } catch (const runtime_error& e) {
            cerr << "Error in Category::get_important_terms: " << e.what() << endl;
            return;
        } catch (const exception& e) {
            cerr << "Error in Category::get_important_terms: " << e.what() << endl;
            return;
        }
    }
}

void Category::print_all_info() {
            ofstream file{CAT_FILENAME, ios::app};

            if (!file) {
                throw runtime_error("File Error in print_all_info");
                return;
            }

            file << "Category: " << conv_cat_type(category_type) << "\n";
            
            for (auto& term : most_important_terms) {
                file << term.first << ": " << term.second << "\n";
            }
            file << "\n";
            file.close();
        }

// initialize categories vector
static vector<Category> init_categories() {
    vector<Category> categories_list;

    for (int i = 0; i < MAX_CATEGORIES; i++)
        categories_list.emplace_back(i);
    
    return categories_list;
}

// parallel
extern void get_single_cat_par(Corpus * corpus, vector<Category>& cats, text_cat_types_ catint) {
    Category cat(catint);
    cat.get_important_terms(corpus);
    
    {
        lock_guard<mutex> lock(mtx);
        cats.emplace_back(std::move(cat));
    }
}

// sequential
extern void get_single_cat_seq(Corpus * corpus, vector<Category>& cats, text_cat_types_ catint) {
    Category cat(catint);
    cat.get_important_terms(corpus);
    cats.emplace_back(std::move(cat));
}


static double cosine_similarity(const unordered_map<string, double>& doc1, const unordered_map<string, double>& doc2) {
    double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0;

    for (const auto& [word, tfidf1] : doc1) {
        // cout << "word: " << word << endl;
        if (doc2.find(word) != doc2.end()) {
            dotProduct += tfidf1 * doc2.at(word);
        }
        norm1 += tfidf1 * tfidf1;
    }
    
    for (const auto& [word, tfidf2] : doc2) {
        // cout << word << endl;
        norm2 += tfidf2 * tfidf2;
    }
    // cout << "Norm1: " << norm1 << "\tNorm2: " << norm2 << "\tdotProduct: " << dotProduct << endl;

    if (fabs(norm1) < 1e-9 || fabs(norm2) < 1e-9) return 0.0; // Avoid division by zero

    return dotProduct / (sqrt(norm1) * sqrt(norm2));
}

extern unknown_class classify_text(const unordered_map<string, double>& unknownText, vector<Category> cat_vect, string correct_type) {
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

    // cout << correct_type << "\t" << conv_cat_type(unknown_classification.classified_type) << endl;
    if (unknown_classification.correct_type == unknown_classification.classified_type) {
        unknown_classification.correct = true;
        // cout << "True";
    } else {
        unknown_classification.correct = false;
        // cout << "False";
    }

    return unknown_classification;
}

extern unknown_classification_s init_classification(Corpus * unknown_corpus, vector<Category> cat_vect, vector<string> correct_types) {
    unknown_classification_s u_classified;
    u_classified.correct_count = 0;
    u_classified.total_count = (*unknown_corpus).documents.size();

    for (int i = 0; i < u_classified.total_count; i++) {
        u_classified.unknown_doc.emplace_back(classify_text((*unknown_corpus).documents.at(i).tf_idf, cat_vect, correct_types.at(i)));
        
        if (u_classified.unknown_doc.at(i).correct)
            u_classified.correct_count++;
    }

    u_classified.correct_db = static_cast<double>(u_classified.correct_count) / u_classified.total_count * 100;

    return u_classified;
}

extern void print_classifications(unknown_classification_s classified) {
    cout << "# of total unknown Documents: " << classified.total_count << endl;
    cout << "# of total correctly Classfied: " << classified.correct_count << endl;
    cout << "% Classifed Correctly: " << classified.correct_db << "%" << endl;
    cout << "Actual\tClassified\tCorrect" << endl;
    
    for (int i = 0; i < classified.total_count; i++) {
        // if (classified.unknown_doc.at(i).correct_type == )
        cout << conv_cat_type(classified.unknown_doc.at(i).correct_type) << "\t" << conv_cat_type(classified.unknown_doc.at(i).classified_type) << "\t";
        
        if (classified.unknown_doc.at(i).correct)
            cout << "True";
        else 
            cout << "False";
        
        cout << endl;
    }
}


// deprecated function
extern vector<Category> get_all_category_important_terms(Corpus * corpus) {
    vector<Category> categories_list = init_categories();
    vector<thread> category_threads;

    for (int i = 0; i < MAX_CATEGORIES; i++) {
        cout << "getting important categories for " << conv_cat_type(conv_cat_type(i)) << endl;
        category_threads.emplace_back([&] {
            categories_list[i].get_important_terms(corpus);
        });
    }

    for (auto& thread : category_threads)
        thread.join();

    return categories_list;
}
