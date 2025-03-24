#include "TFIDF.hpp"

int main() {
    TFIDF::TFIDF_ tf_idf{true, "data/BBC-News-Training.csv", "data/unknown_text.txt", "data/correct_unknown.txt", 
                         "tests/test-output/results/parallel-4-results.txt", 
                         "tests/test-output/processed-data-results/parallel-4-processed.csv",
                         true, true, true, true, true, true, true, 4
                        };
    tf_idf.process_all_data();
    return 0;
}