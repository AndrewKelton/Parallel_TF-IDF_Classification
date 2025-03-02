#include <iostream>
#include <chrono>
#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"
#include "file_operations.h"
#include "categories.h"
#include "utils.h"

using namespace std;

/* RUN THIS ON EUSTIS !! 
 * mac don't know what to do
 */

int main() {
    try {
        convert_results_txt_to_csv(0, false);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    try {
        convert_results_txt_to_csv(1, false);
    } catch (runtime_error e) {
        cerr << "Error converting txt to csv: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
