#include "TFIDF.hpp"
#include <fstream>

int main(int argc, char * argv[]) {
    std::string input_folder{"tests/data/dataset-" + std::to_string(atoi(argv[1])) + "/"};
    std::string input_training{input_folder + "training-data.csv"};
    std::string input_testing_txt{input_folder + "testing-data.txt"};
    std::string input_testing_cat{input_folder + "testing-correct-data.txt"};

    std::string base_output_folder{"tests/output/"}; 
    std::string base_file_name{""};


    if (argc < 3)
        base_file_name += "sequential-" + std::to_string(atoi(argv[1])) + "-";
    else 
        base_file_name += "parallel-" + std::to_string(atoi(argv[2])) + "-" + std::to_string(atoi(argv[1])) + "-";
    
    std::string results_output{base_output_folder + "results/" + base_file_name + "results.txt"};
    std::string logging_output{base_output_folder + "logs/" + base_file_name + "errors.log"};
    std::string procssd_output{base_output_folder + "processed-data-results/" + base_file_name + "processed.csv"};

    std::ofstream out(results_output);
    std::ofstream err(logging_output);

    std::streambuf* coutBuf = std::cout.rdbuf();
    std::streambuf* cerrBuf = std::cerr.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::cerr.rdbuf(err.rdbuf());


    TFIDF::TFIDF_ tfidf{
        argc >= 3, 
        input_training, 
        input_testing_txt, 
        input_testing_cat,
        results_output,
        procssd_output,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        argc >= 3 ? atoi(argv[2]) : 1
    };

    tfidf.process_all_data();

    std::cout.rdbuf(coutBuf);
    std::cerr.rdbuf(cerrBuf);

    return 0;
}