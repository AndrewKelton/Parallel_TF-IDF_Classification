#include "categories.h"

// return sorted vector of tfidf terms
static vector<pair<string, double>> sort_unordered_umap(unordered_map<string, double> terms) {
    vector<pair<string, double>> vectored_umap(terms.begin(), terms.end());

    sort(vectored_umap.begin(), vectored_umap.end(), [](const auto&a, const auto&b) {
        return a.second > b.second;
    });

    return vectored_umap;
}

// return pair for nth important tfidf term in category
static pair<string, double> search_nth_important_term(vector<vector<pair<string, double>>> all_tfidf_terms, vector<pair<string, double>> used) {
    pair<string, double> current_high = all_tfidf_terms[0][0];

    for (auto& row : all_tfidf_terms) {

        /* only checking first 5 terms in a row, 
         * since only need 5 important terms
         */
        for (int i = 0; i < 5; i++) {
            pair<string, double> current_pair = row[i];

            if (current_high.second < current_pair.second || find(used.begin(), used.end(), current_high) != used.end())
                current_high = current_pair;
        }
    }

    return current_high;
} 

// get important terms for category
static void get_important_terms(Corpus * corpus, Category * category, int category_type) {

    vector<vector<pair<string, double>>> vectored_all_umaps; // vectorized sorted tfidf mapping

    for (auto& document : corpus->documents) {
        if (document.category != category_type)
            continue;
        
        vectored_all_umaps.emplace_back(sort_unordered_umap(document.tf_idf));
    }

    for (int i = 0; i < 5; i++)
        category->most_important_terms.emplace_back(search_nth_important_term(vectored_all_umaps, category->most_important_terms));
}

extern void get_all_category_terms(Corpus * corpus, Categories * categories) {
    thread threads[MAX_CATEGORIES];

    for (int i = 0; i < MAX_CATEGORIES; i++) {
        threads[i] = thread(get_important_terms, ref(corpus), ref(categories->categories[i]), i);
    }
}

void Category::get_most_important_terms(Corpus corpus) {
    unordered_map<string, double> tfidf_category_corpus;

    for (auto& document : corpus.documents) {
        if (document.category != this->category)
            continue;

        for (auto& term_idf : document.tf_idf)
            tfidf_category_corpus.
        
    }
}


/* TESTER */
extern void print_a_vectored(unordered_map<string, double> mapped) {
    vector<pair<string, double>> vec = sort_unordered_umap(mapped);

    for (const auto& pair : vec) {
        cout << pair.first << ": " << pair.second << endl;
    }
}
/* TESTER */
