# Parallel TF-IDF Classification

## Documentation
Check out the documentation at my website!
<a href="https://andrewkelton.me/parallel-tf-idf-project/index.html" target="_blank">Documentation</a>

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

### Build Instructions
#### Build Environment
```bash
git clone https://github.com/AndrewKelton/Parallel_TF-IDF_Classification.git
cd Parallel_TF-IDF_Classification

chmod +x scripts/setup.sh
scripts/setup.sh
```
#### Build Environment & Run Tests
```bash
chmod +x scripts/all.sh
scripts/all.sh
```


## Run Tests
### Run Individual Tests
#### a. Parallel Test
```bash
make par-test
```
#### b. Sequential Test
```bash
make seq-test
```
#### c. Parallel & Sequential Test
```bash
make test
```

### Run _n_ Iterations of Run Single Test c
```bash
chmod +x scripts/run.sh
scripts/run.sh
```
_(Runs multiple iterations of make test for benchmarking.)_


## Cleaning the Environment
```bash
chmod +x scripts/cleanup.sh
```


## Challenges
* Efficient parallel version of TF-IDF algorithm
* Optimize thread usage to maximize speedup
* Handle large datasets efficiently
* Ensure correctness in parallel optimization

## Tasks
* Implement parallel vectorization and TF-IDF compution
* Implement classifier using cosine similarity
* Measure times of computation in different sections of code
* Measure accuracy of classification
* Compare Parallel and Sequential times and accuracy

## Goals
* Prove parallel TF-IDF is more efficient than its sequential implementation
* Ensure correctness in parallel TF-IDF computation
* Successfully classify documents using TF-IDF scores


## Notes
* Dataset Requirement: Ensure BBC-News-Training.csv is available in the project directory.
* Logging & Output: Results for _Individual Tests_ are stored in test-output/solo/results/, and logs are saved in test-output/solo/logs/. Results for _Comparison Tests_ are stored in test-output/comparison/results/, and logs are saved in test-output/comparison/logs/.
* Running `scripts/run.sh` may take a long time to finish due to the sequential solution taking quite a bit of time. I recommend reducing the number of iterations in `scripts/run.sh` to 5 if you are running this locally.
