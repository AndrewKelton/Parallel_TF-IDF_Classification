/* document.cpp
 * source file for Document.hpp
 */

#include "Document.hpp"
#include "categories.hpp"
#include <fstream>

namespace docs {

    bool Document::is_term(std::string str) {
        return term_count.find(str) != term_count.end();
    }

    double Document::calculate_term_frequency(std::string term) {
        if (total_terms == 0) 
            return 0.0;

        return static_cast<double>(term_count[term]) / total_terms;
    }

    void Document::calculate_term_frequency_doc() {
        for (auto& [word, count] : term_count) 
            term_frequency[word] = calculate_term_frequency(word);
    }

    void Document::print_all_info() {
        std::ofstream file{DOC_FILENAME, std::ios::app};

        if (!file) {
            std::cerr << "Failed to open file: " << DOC_FILENAME << " Error: " << strerror(errno) << std::endl;
            throw std::runtime_error("File Error in Document::print_all_info");
            return;
        }

        file << "Info for Document id: " << document_id << "\n";
        file << print_category();
        file << print_number_terms();
        file << print_tf_idf();
        
        file.close();
    }

    std::string Document::print_text() const {
        return "Text: " + text + "\n";
    }
    std::string Document::print_number_terms() const {
        return "Number of Terms: " + std::to_string(total_terms) + "\n";
    }
    std::string Document::print_category() const { 
        return "Category: " + category + "\n\n";
    }
    std::string Document::print_tf_idf() const {
        std::string tf_idf_str{"TF-IDF Vectorization: \n"};

        int i = 0;
        for (const auto& pair : tf_idf) {
            if (i % 5 == 0 && i != 0)
                tf_idf_str += "\n";
            else if (i != 0)
                tf_idf_str += "\t";
            tf_idf_str += pair.first + ": " + std::to_string(pair.second) + "\n";
        }
        tf_idf_str += "\n";

        return tf_idf_str;
    }
    std::string Document::print_doc_term_count() const {
        std::string term_count_str{"Number of Times Term Appeared in Document: \n"};
        
        int i = 0;
        for (const auto& pair : term_count) {
            if (i % 5 == 0 && i != 0)
                term_count_str += "\n";
            else if (i != 0)
                term_count_str += "\t";
            term_count_str += pair.first +" :" + std::to_string(pair.second);
            i++;
        }
        term_count_str += "\n";

        return term_count_str;
    }
} // namespace docs
