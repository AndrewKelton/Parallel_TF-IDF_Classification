#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "document.h"

using namespace std;

/* Read the .csv input as whole and create a 
 * Document object for each line read in 
 * from the .csv. Saves the text and category,
 * which means this reads in the training data.
 */
extern void read_csv_to_corpus(Corpus& corpus, const string& file_name);

// read and vectorize the unkown text
extern void read_unkown_text(Corpus& corpus, const string& file_name);

/* Convert the *results.txt file to a csv for python preprocess graphing.
 * Will not check if 'file_name' is one of the preincluded files.
 */
extern void convert_results_txt_to_csv(unsigned int par_or_seq /* 0 = parallel, 1 = sequential*/, bool comp_or_solo);

// returns the name of the input file
extern string get_input_file_name();

#endif