# Parallel TF-IDF Classification

## Documentation
Check out the Doxyfile <a href="https://andrewkelton.me/parallel-tf-idf-project/index.html" target="_blank">documentation</a>!
<!--<a href="https://andrewkelton.me/parallel-tf-idf-project/index.html" target="_blank">Documentation</a> -->

## Introduction
Parallel TF-IDF Classification is a high-performance implementation of the Term Frequency-Inverse Document Frequency (TF-IDF) algorithm for document classification. This project leverages parallel computing to efficiently process large text datasets. This project also compares the times and accuracy between the parallel method and the sequential method. Files use a Doxygen comment format.

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

## Run Tests
### Run Individual Tests
```shell
make main
./main 3 128 # arg1 = dataset, arg2 = thread count
```
_Runs test for any dataset and thread count._
#### a. Parallel Tests
```bash
make n-test # n = number of threads to test
```
_Runs parallel test for dataset $(DS_NUM), check Makefile._
#### b. Sequential Test
```bash
make seq-test
```
_Runs sequential test for dataset $(DS_NUM), check Makefile._

#### c. Parallel & Sequential Tests
```bash
make test 
```
_Runs all parallel & sequential tests for dataset $(DS_NUM), check Makefile._

### Run _n_ Iterations of All Tests
```bash
chmod +x scripts/run.sh
scripts/run.sh
```
_Runs multiple iterations of all tests for benchmarking._


## Cleaning the Environment
```bash
chmod +x scripts/cleanup.sh
./scripts/cleanup.sh
```
_Removes installed dependencies and 'cleans' folders._


## Challenges
* Efficient parallel version of TF-IDF algorithm
* Optimize thread usage to maximize speedup
* Handle large datasets efficiently
* Ensure correctness in parallel optimization

## Tasks
* Implement parallel and sequential vectorization and TF-IDF compution
* Implement classifier using cosine similarity
* Measure times of computation in different sections of code
* Measure accuracy of classification
* Compare Parallel and Sequential times and accuracy

## Goals
* Provide a parallel TF-IDF vectorizer/classifier for efficient performance.
* Prove parallel TF-IDF is more efficient than its sequential implementation.
* Ensure correctness in parallel TF-IDF computation.
* Successfully classify documents using TF-IDF scores with respect to cosine similarity.


## Notes
* Dataset Requirement: 
    * Training data contains the format: category<sub>x</sub>, text<sub>x</sub>
    * Untrained data's category and corresponding text should be in separate files. 
        - Untrained Category → testing-category.txt: category<sub>x</sub>
        - Untrained Text → testing-text.txt: text<sub>x</sub>
* Logging & Output: Results for Tests are stored in tests/output/results/, and logs are saved in tests/output/logs/. 
* Formatted Output: Formatted output in the form of CSV files is located in tests/output/processed-data-results/
* Running `scripts/run.sh` may take a long time to finish due to the sequential and lower-thread solutions taking quite a bit of time. I recommend reducing the number of iterations in `scripts/run.sh` to 5 if you are running this locally and would like to get an average comparison.
