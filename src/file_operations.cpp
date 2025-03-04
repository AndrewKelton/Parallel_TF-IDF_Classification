/* file_operations.cpp
 * source file for file_operations.h
 */

#include "file_operations.h"
#include "categories.h"
#include "utils.h"
#include <fstream>

using namespace std;


/* --- Constants --- */

static const string sl{"/"}; // general purpose slash

/* constants for directory paths */
static const string base_test_dir{"test-output"};
static const string processed_csv_dir{base_test_dir + sl + "processed-data-results"};
static const string comp_test_dir{base_test_dir + sl + "comparison"};
static const string solo_test_dir{base_test_dir + sl + "solo"};
static const string res_test_dir{"results"};

/* constants for .txt files (results) paths */
static const string res_par_txt{comp_test_dir + sl + res_test_dir + sl + "main-test-parallel-results.txt"};
static const string res_seq_txt{comp_test_dir + sl + res_test_dir + sl + "main-test-sequential-results.txt"};

static const string res_par_txt_singleton{solo_test_dir + sl + res_test_dir + sl + "main-test-parallel-results-singleton.txt"};
static const string res_seq_txt_singleton{solo_test_dir + sl + res_test_dir + sl + "main-test-sequential-results-singleton.txt"};

/* constants for processed csv files */
static const string processed_par_csv{processed_csv_dir + sl + "parallel-processed.csv"};
static const string processed_seq_csv{processed_csv_dir + sl + "sequential-processed.csv"};

/* ----------------- */


// return pair of first split from csv 'category, text'
static pair<string, string> split_string(const string& str, const char& splitter) {
    size_t comma_pos = str.find(splitter);

    if (comma_pos == string::npos)
        return {str, ""};
        
    return {str.substr(0, comma_pos), str.substr(comma_pos + 1)};
}

// return a new Document object with inputted text and category
static Document create_document(string text, TEXT_CATEGORY_TYPES category) {
    Document new_document;
    new_document.text = text;
    new_document.category = category;

    return new_document;
}

// main function to read in training data
extern void read_csv_to_corpus(Corpus& corpus, const string& file_name) {
    ifstream file(file_name);

    if (!file.is_open()) {
        throw runtime_error("file cannot be opened...");
    }

    string line;
    int i{0};
    while (getline(file, line)) {
        
        // ignore header
        if (line == "category,text")
            continue;

        pair<string, string> split = split_string(line, ',');
        TEXT_CATEGORY_TYPES category = get_category(split.first);
        
        corpus.documents.push_back(create_document(split.second, category));
        i++;
    }   
    
    corpus.num_of_docs.store(i);

    file.close();
}    

// return .txt file name for results to csv
static string get_txt_name(unsigned int par_or_seq /* 0 = parallel, 1 = sequential*/, bool comp_or_solo) {
    if (par_or_seq != 0 && par_or_seq != 1)
        throw runtime_error("invalid params in 'get_file_name'");
    else if (par_or_seq == 0) {
        return comp_or_solo ? res_par_txt_singleton : res_par_txt;
    }

    return comp_or_solo ? res_seq_txt_singleton : res_seq_txt;
}

// return .csv file name from .txt result file name
static string get_csv_name(const string& txt_file_name) {
    string file_txt_path;

    if (txt_file_name.find("parallel") != string::npos)
        return processed_par_csv;
    return processed_seq_csv;
}

// actually write plain text file results to csv file
static void write_sections_csv(vector<pair<string, string>> pln_txt, const string& txt_file_name, bool comp_or_solo) {
    string file_name = get_csv_name(txt_file_name);
    ofstream file(file_name, ios::app); // append to csv

    if (!file) {
        throw_runtime_error("file error in write_sections_csv");
    } else {
        file.seekp(0, ios::end);
        
        if (file.tellp() == 0) 
            file << "Section,Time (ms)\n";
        
        for (const auto& sec_time : pln_txt) {
            std::string no_ms_time = sec_time.second.substr(1, sec_time.second.find(" ms")); // 'stem' the output
            file << sec_time.first << "," << no_ms_time << "\n";
        }
    }
}

// main function to convert txt to csv
extern void convert_results_txt_to_csv(unsigned int par_or_seq /* 0 = parallel, 1 = sequential*/, bool comp_or_solo) {
    string file_name;

    try {
        file_name = get_txt_name(par_or_seq, comp_or_solo);
    } catch (runtime_error e) {
        cerr << "Error in convert_results_txt_to_csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    ifstream file{file_name};

    if (!file.is_open()) 
        throw_runtime_error("convert_results_txt_to_csv");

    else {
        vector<pair<string, string>> res_pln_txt;
        string line;

        while (getline(file, line)) {
            pair<string, string> split = split_string(line, ':');
            res_pln_txt.emplace_back(split);
        }

        try {
            write_sections_csv(res_pln_txt, file_name, comp_or_solo);
        } catch (runtime_error e) {
            cerr << "Error in convert_results_txt_to_csv: " << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
}