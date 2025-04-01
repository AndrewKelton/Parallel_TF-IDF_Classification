#include "TFIDF.hpp"

int main(int argc, char * argv[]) {    
    if (argc < 3)
        return EXIT_FAILURE;

    int ds_num = atoi(argv[1]);
    std::string dataset = "data/dataset-" + std::to_string(ds_num);

    int th_count = atoi(argv[2]);

    TFIDF::TFIDF_ tf_idf{true, dataset + "/training-data.csv", dataset + "/un-trained-data.txt", dataset + "/un-trained-correct-data.txt", 
                            "tests/test-output/results/parallel-" + std::to_string(th_count) + "-" + std::to_string(ds_num) + "-results.txt", 
                            "tests/test-output/processed-data-results/parallel-" + std::to_string(th_count) + "-" + std::to_string(ds_num) + "-processed.csv",
                            true, true, true, true, true, true, true, th_count
                        };
                        
    tf_idf.process_all_data();

    return 0;
}