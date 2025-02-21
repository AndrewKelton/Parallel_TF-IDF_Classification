#include "categories.h"
#include "document.h"
#include "utils.h"

// return sorted vector of tfidf terms
vector<pair<string, double>> Category::sort_unordered_umap(unordered_map<string, double> terms) {
    // cout << "sort_unordered_umap" << endl;
    if (terms.empty())
        throw_runtime_error("no terms or terms are empty");

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
        throw_runtime_error("empty tfidf in ", text_categories.find(static_cast<TEXT_CATEGORY_TYPES>(this->category_type))->second);
    }
    if (all_tfidf_terms[0].empty()) {
        throw_runtime_error("empty tfidf in ", text_categories.find(static_cast<TEXT_CATEGORY_TYPES>(this->category_type))->second);
    }
    
    pair<string, double> current_high = all_tfidf_terms[0][0];

    for (auto& row : all_tfidf_terms) {

        /* only checking first 5 terms in a row, 
         * since only need 5 important terms
         */
        for (int i = 0; i < min(5, static_cast<int>(row.size())); i++) {
            pair<string, double> current_pair = row[i];

            if (current_high.second < current_pair.second || (find(used.begin(), used.end(), current_high) != used.end()))
                current_high = current_pair;
        }
    }

    return current_high;
} 

// get important terms for category
void Category::get_important_terms(Corpus * corpus) {

    vector<vector<pair<string, double>>> vectored_all_umaps; // vectorized sorted tfidf mapping

    for (auto& document : corpus->documents) {
        if (document.category != category_type)
            continue;
        
        // lock_guard<mutex> lock(mtx);
        try {
            vectored_all_umaps.emplace_back(sort_unordered_umap(document.tf_idf));
        } catch (const runtime_error& e) {
            cout << "Error in Category::get_important_terms: " << e.what() << endl;
            return;
        }
    }

    for (int i = 0; i < 5; i++) {
        // lock_guard<mutex> lock(mtx);
        try {
            most_important_terms.emplace_back(search_nth_important_term(vectored_all_umaps, most_important_terms));
        } catch (const runtime_error& e) {
            cout << "Error in Category::get_important_terms: " << e.what() << endl;
            return;
        }
    }
}

void Category::print_info() {
    cout << text_categories.find(static_cast<TEXT_CATEGORY_TYPES>(category_type))->second << endl;
    
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

// get important terms for each category simultaneously
extern vector<Category> get_all_category_important_terms(Corpus * corpus) {
    // vector<Category> categories_list = init_categories();
    vector<Category> categories_list{0,1,2,3,4};
    vector<thread> category_threads;

    for (int i = 0; i < MAX_CATEGORIES; i++) {
        cout << "getting important categories for " << text_categories.find(static_cast<TEXT_CATEGORY_TYPES>(i))->second << endl;
        category_threads.emplace_back([&] {
            categories_list[i].get_important_terms(corpus);
        });

    }

    for (auto& thread : category_threads)
        thread.join();

    return categories_list;
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
