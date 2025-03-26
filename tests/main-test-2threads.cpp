#include "TFIDF.hpp"

int main(int argc, char * argv[]) {    
    if (argc < 2)
        return EXIT_FAILURE;

    int ds_num = atoi(argv[1]);
    std::string dataset = "data/dataset-" + std::to_string(ds_num);

    TFIDF::TFIDF_ tf_idf{true, dataset + "/training-data.csv", dataset + "/un-trained-data.txt", dataset + "/un-trained-correct-data.txt", 
                            "tests/test-output/results/parallel-2-" + std::to_string(ds_num) + "-results.txt", 
                            "tests/test-output/processed-data-results/parallel-2-" + std::to_string(ds_num) + "-processed.csv",
                            true, true, true, true, true, true, true, 2
                        };
    // TFIDF::TFIDF_ tf_idf{true, "data/dataset-1/training-data.csv", "data/dataset-1/un-trained-data.txt", "data/dataset-1/un-trained-correct-data.txt", 
    //                         "tests/test-output/results/parallel-4-" + std::string(argv[1]) + "-" + std::to_string(ds_num) + "results.txt", 
    //                         "tests/test-output/processed-data-results/parallel-4-processed" + std::string(argv[1]) + ".csv",
    //                         true, true, true, true, true, true, true, 4
    //                     };
    tf_idf.process_all_data();

    return 0;
}