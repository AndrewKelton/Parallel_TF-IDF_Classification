/* flag_handler.hpp
 * handles user print flags and calls the print functions.
 */

#ifndef FLAG_HANDLER_HPP
#define FLAG_HANDLER_HPP

#include "config.hpp" 
#include "document.h"
#include "categories.h"
#include "utils.h"
#include <filesystem>

using namespace std;

// call print function for Corpus
inline static void print_terms_info(Corpus * corp) {
    try {
        corp->print_all_info();
    } catch (runtime_error e) {
        cerr << "Error Printing Corpus: " << e.what() << endl;
    }
}

// call print function for Category(s)
inline static void print_cat_info(vector<Category>& cats) {
    if (filesystem::exists(CAT_FILENAME))
        filesystem::remove(CAT_FILENAME);
    
    ofstream tmp_f{CAT_FILENAME};
    tmp_f.close();

    for (auto& cat : cats) {
        try {
            cat.print_all_info();
        } catch (runtime_error e) {
            cerr << "Error Printing " << conv_cat_type(cat.get_type()) << ": " << e.what() << endl;
        }
    }
}

// call print function for Corpus, Category(s), & Document(s)
inline static void print_lengthy(Corpus * corp, vector<Document> docs, vector<Category>& cats) {
    print_terms_info(corp);

    if (filesystem::exists(DOC_FILENAME))
        filesystem::remove(DOC_FILENAME);

    ofstream tmp_f{DOC_FILENAME};
    tmp_f.close();

    for (auto& doc : docs) {
        try {
            doc.print_all_info();
        } catch (runtime_error e) {
            cerr << "Error Printing Document #" << to_string(doc.document_id) << ": " << e.what() << endl;
        }
    }

    print_cat_info(cats);
}

// handle flags to print features
inline void handle_output_flags(Corpus& corpus, vector<Document>& documents, vector<Category>& cat_vect) {
    #if ENABLE_LENGTHY
    print_lengthy(&corpus, documents, cat_vect);
    #endif

    #if (!ENABLE_LENGTHY && ENABLE_TERMS_INFO)
    print_terms_info(&corpus);
    #endif

    #if (!ENABLE_LENGTHY && ENABLE_CATS_INFO)
    print_cat_info(cat_vect);
    #endif
}


#endif