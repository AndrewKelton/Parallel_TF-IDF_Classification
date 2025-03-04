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

void Category::get_important_terms(Corpus * corpus) {

    this->most_important_terms.reserve(5);                   // reserve 5 slots of memory
    vector<vector<pair<string, double>>> vectored_all_umaps; // vectorized sorted tfidf mapping
    
    // sort all the terms for each Document in the Category
    for (auto& document : corpus->documents) {
        if (document.category != category_type)
            continue;

        try {
            vectored_all_umaps.emplace_back(sort_unordered_umap(document.tf_idf));
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

// void Category::print_all_info() {
//     ofstream file{"test-output/lengthy/category-info.txt"};
// 
//     if (!file) {
//         throw runtime_error("File Error in print_all_info");
//         return;
//     }
// 
//     file << "Category: " << conv_cat_type(category_type) << "\n";
//     
//     for (auto& term : most_important_terms) {
//         file <<term.first << ": " <<  term.second << "\n";
//     }
//     // cout << endl;
// }

// initialize categories vector
static vector<Category> init_categories() {
    vector<Category> categories_list;

    for (int i = 0; i < MAX_CATEGORIES; i++)
        categories_list.emplace_back(i);
    
    return categories_list;
}
// 
// extern text_cat_types_ conv_cat_type(string category) {
//     return categories_text.find(category)->second;
// }

// extern string conv_cat_type(text_cat_types_ category) {
//     return text_categories.find(category)->second;
// }

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
