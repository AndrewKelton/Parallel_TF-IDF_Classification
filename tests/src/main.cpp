/* main.cpp */

#include "TFIDF.hpp"
#include <fstream>

int main(int argc, char * argv[]) {

    /* acquire dataset number */
    std::string input_folder{"tests/data/dataset-" + std::to_string(atoi(argv[1])) + "/"};
    std::string input_training{input_folder + "training-data.csv"};
    std::string input_testing_txt{input_folder + "testing-data.txt"};
    std::string input_testing_cat{input_folder + "testing-correct-data.txt"};

    /* set the output files */
    std::string base_output_folder{"tests/output/"}; 
    std::string base_file_name{""};
    if (argc < 3)
        base_file_name += "sequential-" + std::to_string(atoi(argv[1])) + "-";
    else 
        base_file_name += "parallel-" + std::to_string(atoi(argv[2])) + "-" + std::to_string(atoi(argv[1])) + "-";
    std::string results_output{base_output_folder + "results/" + base_file_name + "results.txt"};
    std::string logging_output{base_output_folder + "logs/" + base_file_name + "errors.log"};
    std::string procssd_output{base_output_folder + "processed-data-results/" + base_file_name + "processed.csv"};

    /* grab std::out and send to files */
    std::ofstream out(results_output);
    std::ofstream err(logging_output);
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::streambuf* cerrBuf = std::cerr.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::cerr.rdbuf(err.rdbuf());

    /* initialize TF-IDF object */
    TFIDF::TFIDF_ tfidf{
        argc >= 3,         // using multithreading?
        input_training,    // training data file
        input_testing_txt, // testing data file
        input_testing_cat, // correct testing categories file
        results_output,    // result output file
        procssd_output,    // processed CSV data output file
        true, // completing all TF-IDF tasks
        true, // classify testing data
        true, // record the program performance
        true, // output the program's performance
        true, // output the testing data classifications
        true, // convert output to processed CSV files
        true, // log errors
        argc >= 3 ? atoi(argv[2]) : 1 // number of threads to use
    };

    tfidf.process_all_data(); // process both training and testing data

    /* close the buffer */
    std::cout.rdbuf(coutBuf);
    std::cerr.rdbuf(cerrBuf);

    return 0;
}