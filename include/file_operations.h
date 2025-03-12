/**
 * @file file_operations.h
 * @author Andrew Kelton
 * @brief Provides file handling utilities for loading, reading, and saving data.
 * 
 * @details This file defines functions for handling file I/O operations, including:
 * - Loading a CSV file into a `Corpus` for training data.
 * - Reading and vectorizing unknown text for classification.
 * - Converting results into a CSV format for Python-based preprocessing and graphing.
 * - Retrieving input file names for processing.
 * 
 * These functions enable efficient management of document data for machine learning applications.
 * 
 * @note Requires all input files to be located in the `data` folder.
 * 
 * @version 0.1
 * @date 2025-03-12
 */

#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "document.h"

using namespace std;

/**
 * @brief Reads a CSV file and loads data into a `Corpus` object.
 * 
 * @details Each line in the CSV is converted into a `Document` object, storing 
 * the text and its corresponding category. This function is used for loading 
 * training data into memory.
 * 
 * @param corpus The `Corpus` object where the documents will be stored.
 * @param file_name The path to the CSV file to be read.
 */
extern void read_csv_to_corpus(Corpus& corpus, const string& file_name);

/**
 * @brief Reads and vectorizes unknown text for classification.
 * 
 * @details This function processes new, unseen text data from a .txt file
 * and converts it into a vectorized format for analysis. 
 * 
 * @param corpus The `Corpus` object where the new document will be stored.
 * @param file_name The path to the file containing unknown text.
 * 
 * @note Requires one document/article per line in the .txt file. Can have
 * more than one document/article, but they must be on seperate lines.
 */
extern void read_unknown_text(Corpus& corpus, const string& file_name);

/**
 * @brief Converts a results `.txt` file to CSV for Python preprocessing and graphing.
 * 
 * @details This function formats result data into a structured CSV file for analysis.  
 * It does not validate whether `file_name` is a predefined result file.
 * 
 * @param par_or_seq Determines execution type (0 = parallel, 1 = sequential).
 * @param comp_or_solo Specifies whether to compare results or process a single dataset.
 */
extern void convert_results_txt_to_csv(unsigned int par_or_seq, bool comp_or_solo);

/**
 * @brief Returns the name of the input file.
 * 
 * @return The name of the file as a string.
 */
extern string get_input_file_name();

/**
 * @brief Reads and returns unknown document categories from `correct_unknown.txt`.
 * 
 * @return A vector of strings containing the correct categories of the 
 * unknown input documents.
 */
extern vector<string> read_unknown_cats();

#endif // FILE_OPERATIONS_H