#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <map>
#include <mutex>
#include <unordered_map>
#include "utils.h"
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

// typedef struct CATEGORIES_S {
//     vector<Category> categories;
//     Corpus corpus;
// } categories_t;

class Category {

    public:
        int category;                                      // category type
        vector<pair<string, double>> most_important_terms; // 5 most important terms in category

        // Category(TEXT_CATEGORY_TYPES category) : category(category) {}


        // constructor for Category
        // Category(TEXT_CATEGORY_TYPES category, Corpus corpus) : category(category) {
        //     get_most_important_terms(corpus);
        // }

        // void get_most_important_terms(Corpus corpus);
};

// extern Category categories[MAX_CATEGORIES];

extern void print_a_vectored(unordered_map<string, double> mapped);

extern void get_all_category_important_terms(vector<Category>& categories, Corpus * corpus);


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