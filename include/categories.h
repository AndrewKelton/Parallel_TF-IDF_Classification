#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <vector>
#include <unordered_map>
#include <fstream>

#include "utils.h"

using namespace std;

#define MAX_CATEGORIES 5

const string CAT_FILENAME = "test-output/lengthy/category-info.txt";
class Corpus; // forward declaration

// Category class for determing category of document
class Category {

    private:

        text_cat_types_ category_type;                     // category type (text_cat_types_)
        int number_of_docs;                                // number of documents in category
        vector<pair<string, double>> most_important_terms; // 5 most important terms in category
    
        /* sort tf-idf pairs from high tf-idf to 
         * low tf-idf for local Category.
         */
        vector<pair<string, double>> sort_unordered_umap(unordered_map<string, double> terms);

        // return nth most important tf-idf term for local Category
        pair<string, double> search_nth_important_term(vector<vector<pair<string, double>>> all_tfidf_terms, vector<pair<string, double>> used);

        void put_tf_idf_all(unordered_map<string, double> doc_tf_idf);

    public:
        unordered_map<string, double> tf_idf_all; // tf-idf terms of all documents in category
        
        // regular constructor
        Category(int category_type) : category_type{static_cast<text_cat_types_>(category_type)} {}

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

        text_cat_types_ get_type() {
            return category_type;
        }
    
        // get important terms for category and save to this->most_important_terms
        void get_important_terms(Corpus * corpus);

        /* -- Print Function -- */
        void print_all_info() {
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
};

/* Get important terms for a Category
 * utilizing 5 total threads (parallel approach). 
 * One thread per Category type. Creates a Category
 * object based on the @param catint
 */
extern void get_single_cat_par(Corpus * corpus, vector<Category>& cats, text_cat_types_ catint);

/* Get important terms for a Category
 * u. one per 
 * Category type.
 */
extern void get_single_cat_seq(Corpus * corpus, vector<Category>& cats, text_cat_types_ catint);


// deprecated function
extern vector<Category> get_all_category_important_terms(Corpus * corpus);

#endif