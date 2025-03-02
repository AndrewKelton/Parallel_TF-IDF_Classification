#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include "document.h"
#include "categories.h"
#include "utils.h"

using namespace std;


// const string sl{"/"}; // slash
// 
// // test directories
// const string base_test_dir{"test-output"};
// const string processed_csv_dir{base_test_dir + sl + "processed-data-results"};
// const string comp_test_dir{base_test_dir + sl + "comparison"};
// const string solo_test_dir{base_test_dir + sl + "solo"};
// const string res_test_dir{/* can be either solo or comp*/ "results"};
// 
// // plaintext results from Makefile in comparison
// const string res_par_txt{comp_test_dir + sl + res_test_dir + sl + "main-test-parallel-results.txt"};
// const string res_seq_txt{comp_test_dir + sl + res_test_dir + sl + "main-test-sequential-results.txt"};
// 
// //plaintext results from Makefile in solo
// const string res_par_txt_singleton{solo_test_dir + sl + res_test_dir + sl + "main-test-parallel-results-singleton.txt"};
// const string res_seq_txt_singleton{solo_test_dir + sl + res_test_dir + sl + "main-test-sequential-results-singleton.txt"};
// 
// // data processed csv files 
// const string processed_par_csv{processed_csv_dir + sl + "parallel-processed.csv"};
// const string processed_seq_csv{processed_csv_dir + sl + "sequential-processed.csv"};


// read csv as whole insert into document in corpus
extern void read_csv_to_corpus(Corpus& corpus, const string& file_name);

// write tfidf values of terms to csv from entire corpus
extern void write_to_csv_tfidf_corpus(Corpus corpus, const string& file_name);

// convert *results.txt file to .csv file for graph processing
extern void convert_results_txt_to_csv(unsigned int par_or_seq /* 0 = parallel, 1 = sequential*/, bool comp_or_solo);


#endif