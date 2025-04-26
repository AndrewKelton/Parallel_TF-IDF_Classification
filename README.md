# Parallel TF-IDF Classification

<!-- ## Documentation
Check out the Doxyfile <a href="https://andrewkelton.me/parallel-tf-idf-project/index.html" target="_blank">documentation</a>!
<a href="https://andrewkelton.me/parallel-tf-idf-project/index.html" target="_blank">Documentation</a> -->

## Introduction
Parallel TF-IDF Classification is a high-performance implementation of the Term Frequency-Inverse Document Frequency (TF-IDF) algorithm for document classification. This project leverages parallel computing to efficiently process and classify large text datasets. This project also compares the times and accuracy between the parallel implementations and the sequential implementation. Files use a Doxygen comment format.

### Challenges, Tasks, and Goals
* Provide a parallel TF-IDF vectorizer and classifier library for efficient performance.
* Implement text classifier using cosine similarity
* Measure times of computation in different sections of the code
* Measure accuracy and precision of text classification
* Successfully classify documents using TF-IDF scores with respect to cosine similarity.
* Ensure correctness in parallel TF-IDF computation.
* Prove the parallel TF-IDF is more efficient and useful than its sequential methodology.


## Installation

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- **CPP_STANDARD** C++17 REQUIRED!!!
- **OleanderStemmingLibrary** (Automatically installed with `scripts/setup.sh`)
- **Linux Environment**, I used:
    - **Linux Environment**: Ubuntu 22.04.5 LTS (Jammy Jellyfish)  
    - **Kernel Version**: 5.15.0-130-generic  
    - **Architecture**: x86_64  
    - **Server**: UCF's eustis3 server

### Build Instructions
```bash
# Clone project
git clone https://github.com/AndrewKelton/Parallel_TF-IDF_Classification.git
cd Parallel_TF-IDF_Classification

# Set up the environment, downloadOleanderStemmingLibrary), ensure directories are present
chmod +x scripts/setup.sh
scripts/setup.sh 
```

## Testing

### Parallel Test
```shell
 $ make test
 $ ./test 3 128 # arg1 = dataset-3, arg2 = 128 threads
```
_Runs multithreaded test for dataset specified, utilizes specified # of threads._

### Sequential Test
```bash
 $ make test
 $ ./test 3 # arg1 = dataset-3
```
_Runs sequential test for dataset specified._

### Run All Tests
```bash
 $ chmod +x scripts/run.sh
 $ scripts/run.sh
``` 
_Runs >= 1 iteration of all tests with all 3 datasets._    
#### Example Output
```bash
-----------------------------------------------
                    Test #1                
-----------------------------------------------

                   Dataset-1                 
* --- * --- * --- * ------- * --- * --- * --- *
Testing w/1    Thread(s)  Passed ✅
...
Testing w/1024 Thread(s)  Passed ✅

                   Dataset-2                 
* --- * --- * --- * ------- * --- * --- * --- *
Testing w/1    Thread(s)  Passed ✅
...
Testing w/1024 Thread(s)  Passed ✅

                   Dataset-3                 
* --- * --- * --- * ------- * --- * --- * --- *
Testing w/1    Thread(s)  Passed ✅
...
Testing w/1024 Thread(s)  Passed ✅

                Testing Report                 
-----------------------------------------------
Total Iterations 1
Passed 33/33    total tests
Passed 30/30    parallel tests
Passed 3/3      sequential tests
```

## Cleaning the Environment
```bash
 $ chmod +x scripts/cleanup.sh
 $ ./scripts/cleanup.sh
```
_Removes installed dependencies and 'cleans' folders._



## Notes
* Dataset Requirement: 
    * Training data contains the format: category<sub>x</sub>, text<sub>x</sub>
    * Untrained data's category and corresponding text should be in separate files. 
        - Untrained Category → testing-category.txt: category<sub>x</sub>
        - Untrained Text → testing-text.txt: text<sub>x</sub>
* Logging & Output: Results for Tests are stored in tests/output/results/, and logs are saved in tests/output/logs/. 
* Formatted Output: Formatted output in the form of CSV files is located in tests/output/processed-data-results/
* Running `scripts/run.sh` may take a long time to finish due to the sequential and lower-thread solutions taking quite a bit of time. I recommend reducing the number of iterations in `scripts/run.sh` to 5 if you are running this locally and would like to get an average comparison.
