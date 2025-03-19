/* file_operations.cpp
 * source file for file_operations.h
 */

#include "file_operations.hpp"
#include "categories.hpp"
#include "utils.hpp"
#include <fstream>

static std::string input_file_name;

/* --- Constants --- */
static const std::string sl{"/"}; // general purpose slash

/* constants for directory paths */
static const std::string base_test_dir{"tests/test-output"};
static const std::string processed_csv_dir{base_test_dir + sl + "processed-data-results"};
static const std::string comp_test_dir{base_test_dir + sl + "comparison"};
static const std::string solo_test_dir{base_test_dir + sl + "solo"};
static const std::string res_test_dir{"results"};

// output end file names
static const std::string par_res_file{"parallel-results.txt"};
static const std::string seq_res_file{"sequential-results.txt"};

/* constants for .txt files (results) paths */
static const std::string res_par_txt{comp_test_dir + sl + res_test_dir + sl + par_res_file};
static const std::string res_seq_txt{comp_test_dir + sl + res_test_dir + sl + seq_res_file};

static const std::string res_par_txt_singleton{solo_test_dir + sl + res_test_dir + sl + par_res_file};
static const std::string res_seq_txt_singleton{solo_test_dir + sl + res_test_dir + sl + seq_res_file};

/* constants for processed csv files */
static const std::string processed_par_csv{processed_csv_dir + sl + "parallel-processed.csv"};
static const std::string processed_seq_csv{processed_csv_dir + sl + "sequential-processed.csv"};
/* --- Constants --- */


// return std::pair of first split from csv 'category, text'
static std::pair<std::string, std::string> split_string(const std::string& str, const char& splitter) {
    size_t comma_pos = str.find(splitter);

    if (comma_pos == std::string::npos)
        return {str, ""};
        
    return {str.substr(0, comma_pos), str.substr(comma_pos + 1)};
}

// return a new docs::Document object with inputted text and category
static docs::Document create_document(std::string text, text_cat_types_ category) {
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
        text_cat_types_ category = conv_cat_type(split.first);
        
        corpus.documents.push_back(create_document(split.second, category));
        i++;
    }   
    
    corpus.num_of_docs.store(i);
    file.close();
}    

// return .txt file name for results to csv
static std::string get_txt_name(unsigned int par_or_seq /** 0 = parallel, 1 = sequential*/, bool comp_or_solo) {
    if (par_or_seq != 0 && par_or_seq != 1)
        throw std::runtime_error("invalid params in 'get_file_name'");
    else if (par_or_seq == 0) {
        return comp_or_solo ? res_par_txt_singleton : res_par_txt;
    }

    return comp_or_solo ? res_seq_txt_singleton : res_seq_txt;
}

// return .csv file name from .txt result file name
static std::string get_csv_name(const std::string& txt_file_name) {
    std::string file_txt_path;

    if (txt_file_name.find("parallel") != std::string::npos)
        return processed_par_csv;
    return processed_seq_csv;
}

// actually write plain text file results to csv file
static void write_sections_csv(std::vector<std::pair<std::string, std::string>> pln_txt, const std::string& txt_file_name, bool comp_or_solo) {
    std::string file_name = get_csv_name(txt_file_name);
    std::ofstream file(file_name, std::ios::app); // append to csv

    if (!file) {
        throw_runtime_error("error writing to csv");
    } else {
        file.seekp(0, std::ios::end);
        
        if (file.tellp() == 0) 
            file << "Section,Time (ms)\n";
        
        for (const auto& sec_time : pln_txt) {
            std::string no_ms_time = sec_time.second.substr(1, sec_time.second.find(" ms")); // 'stem' the output
            file << sec_time.first << "," << no_ms_time << "\n";
        }
    }
}

// main function to convert txt to csv
extern void convert_results_txt_to_csv(unsigned int par_or_seq, bool comp_or_solo) {
    std::string file_name;

    try {
        file_name = get_txt_name(par_or_seq, comp_or_solo);
    } catch (std::runtime_error e) {
        std::cerr << "Cannot open file convert_results_txt_to_csv: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file{file_name};

    if (!file.is_open()) 
        throw_runtime_error("convert_results_txt_to_csv");

    else {
        std::vector<std::pair<std::string, std::string>> res_pln_txt;
        std::pair<std::string, std::string> percent_to_end;
        std::string line;

        while (getline(file, line)) {
            
            // ignore non time data
            if (line.find(':') == std::string::npos || line.find('#') != std::string::npos)
                continue;
            
            if (line.find('%') != std::string::npos) {
                std::pair<std::string, std::string> split = split_string(line, ':');
                percent_to_end = std::pair<std::string, std::string>{"Accuracy", split.second.substr(0, split.second.size() - 1)};
                continue;
            }

            std::pair<std::string, std::string> split = split_string(line, ':');
            res_pln_txt.emplace_back(split);
        }
        res_pln_txt.emplace_back(percent_to_end);

        try {
            write_sections_csv(res_pln_txt, file_name, comp_or_solo);
        } catch (std::runtime_error e) {
            std::cerr << "Error in convert_results_txt_to_csv: " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
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

        corpus.documents.push_back(create_document(line, invalid_t_));
        i++;
    }

    corpus.num_of_docs.store(i);
    file.close();
}

extern std::vector<std::string> read_unknown_cats() {
    std::vector<std::string> correct_cats;
    std::ifstream file{"data/correct_unknown.txt"};
    
    if (!file.is_open())
        throw std::runtime_error("file cannot be opened...");

    std::string line;
    while (getline(file, line)) {
        correct_cats.emplace_back(line);
    }

    return correct_cats;
}