/* file_operations.cpp
 * source file for file_operations.hpp
 */

#include "file_operations.hpp"
#include "categories.hpp"
#include "utils.hpp"
#include <fstream>

static std::string input_file_name;

/* --- Constants --- */
// static const std::string sl{"/"}; // general purpose slash
// 
// /* constants for directory paths */
// static const std::string base_test_dir{"tests/output"};
// static const std::string processed_csv_dir{base_test_dir + sl + "processed-data-results"};
// static const std::string comp_test_dir{base_test_dir + sl + "comparison"};
// static const std::string solo_test_dir{base_test_dir + sl + "solo"};
// static const std::string res_test_dir{"results"};
// 
// // output end file names
// static const std::string par_res_file{"parallel-" + std::to_string(ds_num) + "results.txt"};
// static const std::string seq_res_file{"sequential-" + std::to_string(ds_num) + "results.txt"};
// 
// /* constants for .txt files (results) paths */
// static const std::string res_par_txt{comp_test_dir + sl + res_test_dir + sl + par_res_file};
// static const std::string res_seq_txt{comp_test_dir + sl + res_test_dir + sl + seq_res_file};
// 
// static const std::string res_par_txt_singleton{solo_test_dir + sl + res_test_dir + sl + par_res_file};
// static const std::string res_seq_txt_singleton{solo_test_dir + sl + res_test_dir + sl + seq_res_file};
// 
// /* constants for processed csv files */
// static const std::string processed_par_csv{processed_csv_dir + sl + "parallel-processed.csv"};
// static const std::string processed_seq_csv{processed_csv_dir + sl + "sequential-processed.csv"};
/* --- Constants --- */


// return std::pair of first split from csv 'category, text'
static std::pair<std::string, std::string> split_string(const std::string& str, const char& splitter) {
    size_t comma_pos = str.find(splitter);

    if (comma_pos == std::string::npos)
        return {str, ""};
        
    return {str.substr(0, comma_pos), str.substr(comma_pos + 1)};
}

// return a new docs::Document object with inputted text and category
static docs::Document create_document(std::string text, std::string category) {
    docs::Document new_document;
    new_document.text = text;
    new_document.category = category;

    return new_document;
}

// main function to read in training data
extern void read_csv_to_corpus(corpus::Corpus& corpus, const std::string& file_name) {
    
    // set the global input file name
    input_file_name = file_name.substr(file_name.find('/') + 1);
    input_file_name = input_file_name.substr(0, input_file_name.find('.'));

    std::ifstream file(file_name);

    if (!file.is_open()) {
        throw std::runtime_error("file cannot be opened...");
    }

    std::string line;
    int i{0};
    while (getline(file, line)) {
        
        // ignore header
        if (line == "category,text")
            continue;

        std::pair<std::string, std::string> split = split_string(line, ',');
        std::string category = split.first;

        if (corpus.category_types_set.insert(category).second) {
            corpus.num_of_categories++;
        }

        corpus.documents.push_back(create_document(split.second, split.first));
        i++;
    }   
    
    corpus.num_of_docs.store(i);
    file.close();
}    

extern std::string get_input_file_name() {
    return input_file_name;
}

extern void read_unknown_text(corpus::Corpus& corpus, const std::string& file_name) {
    std::ifstream file{file_name};

    if (!file.is_open())
        throw std::runtime_error("file cannot be opened...");
    
    std::string line;
    int i{0};
    while (getline(file, line)) {

        corpus.documents.push_back(create_document(line, ""));
        i++;
    }

    corpus.num_of_docs.store(i);
    file.close();
}

extern std::vector<std::string> read_unknown_cats(const std::string& file_name) {
    std::vector<std::string> correct_cats;
    std::ifstream file{file_name};

    if (!file.is_open())
        throw std::runtime_error("File cannot be opened: " + file_name);

    std::string line;
    while (getline(file, line)) {
        correct_cats.emplace_back(line);
    }

    return correct_cats;
}

extern void convert_results_txt_to_csv(const std::string& txt_file_name, const std::string& csv_file_name) {
    std::ifstream file{txt_file_name};
    if (!file.is_open()) 
        throw std::runtime_error("Cannot open file: " + txt_file_name);

    std::vector<std::pair<std::string, std::string>> res_pln_txt;
    std::pair<std::string, std::string> percent_to_end;
    std::string line;

    while (getline(file, line)) {
        if (line.find(':') == std::string::npos || line.find('#') != std::string::npos)
            continue;

        if (line.find('%') != std::string::npos) {
            std::pair<std::string, std::string> split = split_string(line, ':');
            percent_to_end = {"Accuracy", split.second.substr(0, split.second.size() - 1)};
            continue;
        }

        res_pln_txt.emplace_back(split_string(line, ':'));
    }
    res_pln_txt.emplace_back(percent_to_end);
    file.close();

    std::ofstream out_file(csv_file_name, std::ios::app);
    if (!out_file) 
        throw std::runtime_error("Error writing to CSV: " + csv_file_name);

    out_file.seekp(0, std::ios::end);
    if (out_file.tellp() == 0)
        out_file << "Vectorization,TF-IDF,Categories,Unknown Classification,Accuracy\n";

    std::unordered_map<std::string, std::string> section_mapping;
    for (const auto& sec_time : res_pln_txt) {
        std::string no_ms_time = sec_time.second.substr(1, sec_time.second.find(" ms") - 1);
        section_mapping[sec_time.first] = no_ms_time;
    }

    out_file << section_mapping["Vectorization"] << ","
             << section_mapping["TF-IDF"] << ","
             << section_mapping["Categories"] << ","
             << section_mapping["Unknown Classification"] << ","
             << section_mapping["Accuracy"] << std::endl;
}
