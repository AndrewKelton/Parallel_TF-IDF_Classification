#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <map>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <exception>
// #include "utils.h"
// #include "document.h"

using namespace std;

#define MAX_CATEGORIES 5


class Corpus; // forward declaration


// enum of text classifcation categories
enum TEXT_CATEGORY_TYPES {
    sport_t, business_t,
    politics_t, tech_t,
    entertainment_t
};

// mapping of enum type to classifcation type
const map<string, TEXT_CATEGORY_TYPES> categories_text = {
    {"sport", sport_t},
    {"business", business_t},
    {"politics", politics_t},
    {"tech", tech_t},
    {"entertainment", entertainment_t}
};

const map<TEXT_CATEGORY_TYPES, string> text_categories = {
    {sport_t, "sport"},
    {business_t, "business"},
    {politics_t, "politics"},
    {tech_t, "tech"},
    {entertainment_t, "entertainment"}
};


// Category class for determing category of document
class Category {

    private:
        mutex mtx;
        int category_type;                                 // category type
        vector<pair<string, double>> most_important_terms; // 5 most important terms in category

        vector<pair<string, double>> sort_unordered_umap(unordered_map<string, double> terms);
        pair<string, double> search_nth_important_term(vector<vector<pair<string, double>>> all_tfidf_terms, vector<pair<string, double>> used);

    public:
        
        // regular constructor
        Category(int category_type) : category_type{category_type} {}
        // Category(const Category&) = delete;  // Copy constructor deleted
        // Category(Category&&) = default; 
        /* constructors for vector */
        Category(const Category&) = default;
        Category& operator=(const Category&) = delete;  

        Category(Category&& other) noexcept
            : category_type{other.category_type},
            most_important_terms{std::move(other.most_important_terms)} 
        {}
        Category& operator=(Category&& other) noexcept {
            if (this != &other) {
                category_type = other.category_type;
                most_important_terms = std::move(other.most_important_terms);
            }
            return *this;
        }


        void get_important_terms(Corpus * corpus);

        void print_info();
};

// extern Category categories[MAX_CATEGORIES];

// extern void print_a_vectored(unordered_map<string, double> mapped);

extern vector<Category> get_all_category_important_terms(/*vector<Category>& categories, */Corpus * corpus);

extern TEXT_CATEGORY_TYPES get_category(string category);
extern string get_category(int category);

extern void get_single_cat(Corpus * corpus, vector<Category>& cats, int catint);

// 
// class Categories {
// 
//     public:
//         Corpus corpus;
//         vector<Category> categories;
//         // Categories(Corpus corpus) : corpus(corpus) {}
//         // Categories() {
//         //     for (int i = 0; i < MAX_CATEGORIES; i++) {
//         //        categories.emplace_back(static_cast<TEXT_CATEGORY_TYPES>(i), corpus);
//         //     }
//         // }
// 
//         // constructor for Categories
//         // Categories(Corpus corpus) {
//         //     for (int i = 0; i < MAX_CATEGORIES; i++) {
//         //        categories.emplace_back(static_cast<TEXT_CATEGORY_TYPES>(i), corpus);
//         //     }
//         // }
// };

// class Category : public Categories {
// 
//     public:
//         int category;                                      // category
//         vector<pair<string, double>> most_important_terms; // 5 most important terms in category
// 
//         Category(TEXT_CATEGORY_TYPES category) : category(category) {}
// 
// 
//         // constructor for Category
//         // Category(TEXT_CATEGORY_TYPES category, Corpus corpus) : category(category) {
//         //     get_most_important_terms(corpus);
//         // }
// 
//         void get_most_important_terms(Corpus corpus);
// 
// };










// const vector<string> categories_text = {
//     "sport", "business", 
//     "politics", "tech",
//     "entertainent"
// };


// // class of category types
// class Categories {
//     // int flag;
// 
//     const vector<string> categories_text = {
//         "sport", "business", 
//         "politics", "tech",
//         "entertainent"
//     };
// 
//     public:
//         //  
// 
//         enum TEXT_CATEGORY_TYPES {
//             sport_t, business_t,
//             politics_t, tech_t,
//             entertainment_t
//         };
// 
//         enum REVIEW_CATEGORY_TYPES {
//             negative_t, positive_t, neutral_t
//         };
// 
//         enum CATEGORY_TYPES {
//             sport_negative_t = sport_t | negative_t,
//             business_positive_t = business_t | positive_t,
//             politics_neutral_t = politics_t | neutral_t,
//             tech_neutral_t = tech_t, entertainment_neutral_t = entertainment_t
//         };
// 
// //         typedef struct TEXT_CATEGORY_INFO {
// //             TEXT_CATEGORY_TYPES classification;
// // 
// //         };
// 
// // auto get_categories() {
// //     switch (flag) {
// //         case review_t:
// //             return Categories::TEXT_CATEGORY_TYPES;
// //     }   
// // }
// };

#endif