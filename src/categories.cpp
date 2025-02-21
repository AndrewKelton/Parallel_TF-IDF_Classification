#include "categories.h"
#include "document.h"
#include "utils.h"
#include <mutex>
#include <thread>

mutex mtx;

// return sorted vector of tfidf terms
vector<pair<string, double>> Category::sort_unordered_umap(unordered_map<string, double> terms) {
    // cout << "sort_unordered_umap" << endl;
    if (terms.empty())
        throw_runtime_error("no terms or terms are empty in ", get_category(this->category_type));

    vector<pair<string, double>> vectored_umap(terms.begin(), terms.end());

    sort(vectored_umap.begin(), vectored_umap.end(), [](const auto&a, const auto&b) {
        return a.second > b.second;
    });

    return vectored_umap;
}

// return pair for nth important tfidf term in category
pair<string, double> Category::search_nth_important_term(vector<vector<pair<string, double>>> all_tfidf_terms, vector<pair<string, double>> used) {
    // cout << "search_nth_important_term" << endl;

    if (all_tfidf_terms.empty()){
        throw_runtime_error("empty tfidf in ", get_category(this->category_type));
    }
    if (all_tfidf_terms[0].empty()) {
        throw_runtime_error("empty tfidf in ", get_category(this->category_type));
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

// get important terms for category
void Category::get_important_terms(Corpus * corpus) {

    // cout << "Getting important terms for " << get_category(this->category_type) << endl;

    this->most_important_terms.reserve(5);
    vector<vector<pair<string, double>>> vectored_all_umaps; // vectorized sorted tfidf mapping
    
    for (auto& document : corpus->documents) {
        if (document.category != category_type)
            continue;
        // cout << document.document_id;
        try {
            vectored_all_umaps.emplace_back(sort_unordered_umap(document.tf_idf));
        } catch (const runtime_error& e) {
            cout << "Error Category::get_important_terms: " << e.what() << endl;
            break;
        } catch (const exception& e) {
            cout << "Error Category::get_important_terms: " << e.what() << endl;
            break;
        }
    }

    for (int i = 0; i < 5; i++) {
        try {
            most_important_terms.emplace_back(search_nth_important_term(vectored_all_umaps, most_important_terms));
        } catch (const runtime_error& e) {
            cout << "Error in Category::get_important_terms: " << e.what() << endl;
            return;
        } catch (const exception& e) {
            cout << "Error in Category::get_important_terms: " << e.what() << endl;
            return;
        }
    }
    // cout << "Successfully created imortant terms for " << get_category(this->category_type) << endl;
}

void Category::print_info() {
    cout << "category_type: " << category_type << endl;
    cout << get_category(this->category_type) << endl;
    
    for (auto& term : most_important_terms) {
        cout <<term.first << ": " <<  term.second << endl;
    }
    cout << endl;
}

// initialize categories vector
static vector<Category> init_categories() {
    vector<Category> categories_list;

    for (int i = 0; i < MAX_CATEGORIES; i++)
        categories_list.emplace_back(i);
    
    return categories_list;
}

extern void get_single_cat(Corpus * corpus, vector<Category>& cats, int catint) {
    Category cat(catint);
    cat.get_important_terms(corpus);
    
    {
        lock_guard<mutex> lock(mtx);
        cats.emplace_back(std::move(cat));
    }
}

extern void get_single_cat_seq(Corpus * corpus, vector<Category>& cats, int catint) {
    Category cat(catint);
    cat.get_important_terms(corpus);
    cats.emplace_back(std::move(cat));
}

// get important terms for each category simultaneously
extern vector<Category> get_all_category_important_terms(Corpus * corpus) {
    vector<Category> categories_list = init_categories();
    vector<thread> category_threads;

    for (int i = 0; i < MAX_CATEGORIES; i++) {
        cout << "getting important categories for " << get_category(i) << endl;
        category_threads.emplace_back([&] {
            categories_list[i].get_important_terms(corpus);
        });
    }

    for (auto& thread : category_threads)
        thread.join();

    return categories_list;
}

extern TEXT_CATEGORY_TYPES get_category(string category) {
    return categories_text.find(category)->second;
}

extern string get_category(int category) {
    return text_categories.find(static_cast<TEXT_CATEGORY_TYPES>(category))->second;
}


// get important terms for each category simultaneously
// extern void get_all_category_terms(Corpus * corpus, Categories * categories) {
//     thread threads[MAX_CATEGORIES];
// 
//     for (int i = 0; i < MAX_CATEGORIES; i++) {
//         threads[i] = thread(get_important_terms, ref(corpus), ref(categories->categories[i]), i);
//     }
// }



// void Category::get_most_important_terms(Corpus corpus) {
//     unordered_map<string, double> tfidf_category_corpus;
// 
//     for (auto& document : corpus.documents) {
//         if (document.category != this->category)
//             continue;
// 
//         for (auto& term_idf : document.tf_idf)
//             tfidf_category_corpus.
//         
//     }
// }


/* TESTER */
// extern void print_a_vectored(unordered_map<string, double> mapped) {
//     vector<pair<string, double>> vec = sort_unordered_umap(mapped);
// 
//     for (const auto& pair : vec) {
//         cout << pair.first << ": " << pair.second << endl;
//     }
// }
/* TESTER */
