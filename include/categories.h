#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "utils.h"
#include <map>

using namespace std;

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