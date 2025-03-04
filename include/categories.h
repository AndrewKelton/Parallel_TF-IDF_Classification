#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <map>
#include <thread>
#include <vector>
#include <unordered_map>

#include "utils.h"

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

        int category_type;                                 // category type (TEXT_CATEGORY_TYPES)
        vector<pair<string, double>> most_important_terms; // 5 most important terms in category

        /* sort tf-idf pairs from high tf-idf to 
         * low tf-idf for local Category.
         */
        vector<pair<string, double>> sort_unordered_umap(unordered_map<string, double> terms);

        // return nth most important tf-idf term for local Category
        pair<string, double> search_nth_important_term(vector<vector<pair<string, double>>> all_tfidf_terms, vector<pair<string, double>> used);

    public:
        
        // regular constructor
        Category(int category_type) : category_type{category_type} {}

        /* constructors for vector support */
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
    
        // get important terms for category and save to this->most_important_terms
        void get_important_terms(Corpus * corpus);

        // print important category info
        void print_info();
};

// return enum representation of string category
extern TEXT_CATEGORY_TYPES get_category(string category);

// return string representation of enum category
extern string get_category(int category);

/* Get important terms for a Category
 * utilizing 5 total threads (parallel approach). 
 * One thread per Category type. Creates a Category
 * object based on the @param catint
 */
extern void get_single_cat_par(Corpus * corpus, vector<Category>& cats, int catint);

/* Get important terms for a Category
 * u. one per 
 * Category type.
 */
extern void get_single_cat_seq(Corpus * corpus, vector<Category>& cats, int catint);


// deprecated function
extern vector<Category> get_all_category_important_terms(Corpus * corpus);

#endif