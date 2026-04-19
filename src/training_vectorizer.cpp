/* training_vectorizer.cpp
 * source file for training_vectorizer.hpp
 */

#include "training_vectorizer.hpp"
#include "preprocess.hpp"
#include "categories.hpp"


void build_vocaulary(corpus::Corpus * corpus) {
    int id = 0;
    std::vector<std::string> tokens;

    for (auto& document : (*corpus).documents) {
        tokens = preprocess_tokenize(&document);
        document.total_terms = tokens.size();
        
        // add to vocab + term idx if not seen yet 
        for (auto& token: tokens) {
            if (corpus->term_to_index.count(token) == 0) {
                int new_idx = corpus->vocabulary.size();
                corpus->vocabulary.push_back(token);
                corpus->term_to_index[token] = new_idx;
            }

            document.term_count[token] += 1; // count of term t in document d
        }
    }
}