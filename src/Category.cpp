/* Category.cpp
 * source file for Category.hpp
 */

#include "Category.hpp"
#include <unordered_map>

std::string Category::get_type() {
    return category_type;
}

std::vector<double> Category::get_centroid() {
    return centroid;
}

extern std::vector<Category> build_categories(const corpus::Corpus& corpus) {
    std::vector<Category> categories;

    std::unordered_map<std::string, std::vector<size_t>> category_type_to_indices;
    int num_of_docs = corpus.documents.size();

    for (size_t row = 0; row < num_of_docs; ++row) {
        const std::string& label = corpus.documents[row].category;
        category_type_to_indices[label].push_back(row);
    }

    int row_count = corpus.tfidf_matrix.size();
    int col_count = corpus.tfidf_matrix[0].size();
    for (auto& category : category_type_to_indices) {
        std::vector<size_t> indices = category.second;
        std::vector<double> centroid(row_count, 0.0);
        int num_docs_category = indices.size();

        for (int i = 0; i < row_count; ++i) {
            double term_tfidf_total = 0.0;

            for (auto& doc_idx: indices) {
                term_tfidf_total += corpus.tfidf_matrix[i][doc_idx];
            }

            centroid[i] = term_tfidf_total / num_docs_category;
        }

        Category new_category(category.first, centroid);
        categories.emplace_back(new_category);
    }

    return categories;
}