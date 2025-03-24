#include "TFIDF.hpp"

int main() {
    TFIDF::TFIDF_ tf_idf{false, "data/BBC-News-Training.csv", "data/unknown_text.txt", "data/correct_unknown.txt", 
                         "tests/test-output/results/sequential-results.txt", 
                         "tests/test-output/processed-data-results/sequential-processed.csv",
                         true, true, true, true, true, true, true, -1
                        };
    tf_idf.process_all_data();
    return 0;
}