#include "TFIDF.hpp"

int main() {
    TFIDF::TFIDF_ tf_idf{false};
    tf_idf.process_all_data();
    return 0;
}