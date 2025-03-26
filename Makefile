# Makefile for tests

# Compiler and Flags
CXX = g++
CXXFLAGS += -std=c++17 -g -Wall -Wextra -pthread \
		   -I/opt/homebrew/include -I/opt/homebrew/include/ \
		   -Iinclude -Iinclude/OleanderStemmingLibrary/src \
		   -Wdeprecated-declarations -Wno-unused-function \
		   -Wno-unused-variable -Wno-unused-parameter \
		   -Wno-catch-value -Wno-unused-value \
		   -Wno-sign-compare -Wno-unused-but-set-variable

# Dataset number, change to 1,2,3 if using datest-1,dataset-2,dataset-3
DS_NUM = 3

# Project directories #
SRC_DIR = src
TST_DIR = tests
OBJ_DIR = obj
BUILD_DIR = build

# Test directories
OUTPUT_DIR = test-output

# Create necessary build directories
$(shell mkdir -p $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR) $(TST_DIR)/$(BUILD_DIR))


# Common source files
COMMON_SOURCES = $(SRC_DIR)/count_vectorization.cpp \
				 $(SRC_DIR)/categories.cpp \
                 $(SRC_DIR)/document.cpp \
                 $(SRC_DIR)/preprocess.cpp \
                 $(SRC_DIR)/file_operations.cpp \
				 $(SRC_DIR)/TFIDF.cpp 

COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(COMMON_SOURCES))			 


# Parallel test source
# PARALLEL_SOURCES = $(TST_DIR)/main-test-parallel.cpp $(COMMON_SOURCES)
# PARALLEL_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_SOURCES))
# PARALLEL_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-parallel

PARALLEL_2_SOURCES = $(TST_DIR)/main-test-2threads.cpp $(COMMON_SOURCES)
PARALLEL_2_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_2_SOURCES))
PARALLEL_2_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-2threads

PARALLEL_4_SOURCES = $(TST_DIR)/main-test-4threads.cpp $(COMMON_SOURCES)
PARALLEL_4_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_4_SOURCES))
PARALLEL_4_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-4threads

PARALLEL_8_SOURCES = $(TST_DIR)/main-test-8threads.cpp $(COMMON_SOURCES)
PARALLEL_8_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_8_SOURCES))
PARALLEL_8_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-8threads

PARALLEL_16_SOURCES = $(TST_DIR)/main-test-16threads.cpp $(COMMON_SOURCES)
PARALLEL_16_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_16_SOURCES))
PARALLEL_16_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-16threads

PARALLEL_32_SOURCES = $(TST_DIR)/main-test-32threads.cpp $(COMMON_SOURCES)
PARALLEL_32_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_32_SOURCES))
PARALLEL_32_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-32threads

PARALLEL_64_SOURCES = $(TST_DIR)/main-test-64threads.cpp $(COMMON_SOURCES)
PARALLEL_64_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_64_SOURCES))
PARALLEL_64_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-64threads

PARALLEL_128_SOURCES = $(TST_DIR)/main-test-128threads.cpp $(COMMON_SOURCES)
PARALLEL_128_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_128_SOURCES))
PARALLEL_128_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-128threads

PARALLEL_256_SOURCES = $(TST_DIR)/main-test-256threads.cpp $(COMMON_SOURCES)
PARALLEL_256_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_256_SOURCES))
PARALLEL_256_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-256threads

PARALLEL_512_SOURCES = $(TST_DIR)/main-test-512threads.cpp $(COMMON_SOURCES)
PARALLEL_512_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_512_SOURCES))
PARALLEL_512_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-512threads

PARALLEL_1024_SOURCES = $(TST_DIR)/main-test-1024threads.cpp $(COMMON_SOURCES)
PARALLEL_1024_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_1024_SOURCES))
PARALLEL_1024_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-1024threads

THREADS_LIST = 2 4 8 16 32 64 128 256 512 1024
PARALLEL_EXECUTABLES = \
	$(TST_DIR)/$(BUILD_DIR)/main-test-2threads \
	$(TST_DIR)/$(BUILD_DIR)/main-test-4threads \
    $(TST_DIR)/$(BUILD_DIR)/main-test-8threads \
    $(TST_DIR)/$(BUILD_DIR)/main-test-16threads \
    $(TST_DIR)/$(BUILD_DIR)/main-test-32threads \
    $(TST_DIR)/$(BUILD_DIR)/main-test-64threads \
    $(TST_DIR)/$(BUILD_DIR)/main-test-128threads \
	$(TST_DIR)/$(BUILD_DIR)/main-test-256threads \
	$(TST_DIR)/$(BUILD_DIR)/main-test-512threads \
	$(TST_DIR)/$(BUILD_DIR)/main-test-1024threads


# Non-Optimized Parallel test source
# NON_OPT_PARALLEL_SOURCES = $(TST_DIR)/main-test-non-optimized-parallel.cpp $(COMMON_SOURCES)
# NON_OPT_PARALLEL_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(NON_OPT_PARALLEL_SOURCES))
# NON_OPT_PARALLEL_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-non-optimized-parallel

# Sequential test source 
SEQUENTIAL_SOURCES = $(TST_DIR)/main-test-sequential.cpp $(COMMON_SOURCES)
SEQUENTIAL_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(SEQUENTIAL_SOURCES))
SEQUENTIAL_EXEC = $(TST_DIR)/$(BUILD_DIR)/main-test-sequential


# Target executable names 
TARGET_PAR = main-par
TARGET_SEQ = main-seq
# TARGET_NON = main-non-opt

# Build targets for the parallel executables with different threads
all: $(PARALLEL_2_EXEC) $(PARALLEL_4_EXEC) $(PARALLEL_8_EXEC) $(PARALLEL_16_EXEC) $(PARALLEL_32_EXEC) $(PARALLEL_64_EXEC) $(PARALLEL_128_EXEC) $(PARALLEL_256_EXEC) $(PARALLEL_512_EXEC) $(PARALLEL_1024_EXEC) $(SEQUENTIAL_EXEC) 

$(PARALLEL_2_EXEC): $(PARALLEL_2_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_4_EXEC): $(PARALLEL_4_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_8_EXEC): $(PARALLEL_8_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_16_EXEC): $(PARALLEL_16_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_32_EXEC): $(PARALLEL_32_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_64_EXEC): $(PARALLEL_64_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_128_EXEC): $(PARALLEL_128_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_256_EXEC): $(PARALLEL_256_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_512_EXEC): $(PARALLEL_512_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(PARALLEL_1024_EXEC): $(PARALLEL_1024_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(NON_OPT_PARALLEL_EXEC): $(NON_OPT_PARALLEL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SEQUENTIAL_EXEC): $(SEQUENTIAL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^	

# Executables with no automatic input
main-seq: $(SEQUENTIAL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
$(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# output files
PARALLEL_RES = results/parallel-results.txt
PARALLEL_LOG = logs/parallel-errors.log
SEQUENTIAL_RES = results/sequential-results.txt
SEQUENTIAL_LOG = logs/sequential-errors.log

# Comparison tester, run both parallel.cpp & sequential.cpp
test: $(PARALLEL_EXEC) $(SEQUENTIAL_EXEC)
	@echo "Running parallel tests with different thread configurations..."
	@for threads in $(THREADS_LIST); do \
		echo "Running main-test-$$threads with input from data/dataset-$(DS_NUM)..."; \
		./$(TST_DIR)/$(BUILD_DIR)/main-test-$$threads $(DS_NUM) > $(TST_DIR)/$(OUTPUT_DIR)/results/parallel-$$-$(DS_NUM)-results.txt 2> $(TST_DIR)/$(OUTPUT_DIR)/logs/parallel-$$-errors.log; \
		echo "main-test-$$threads results saved in $(TST_DIR)/$(OUTPUT_DIR)/results/parallel-$$-$(DS_NUM)-results.txt"; \
		echo "main-test-$$threads error logs saved in $(TST_DIR)/$(OUTPUT_DIR)/logs/parallel-$$-$(DS_NUM)-errors.log"; \
	done
# 
# 	@echo "Running main-test-parallel with input from data/dataset-$(DS_NUM)..."
# 	@./$(PARALLEL_EXEC) $(DS_NUM) > $(TST_DIR)/$(OUTPUT_DIR)/$(PARALLEL_RES) 2> $(TST_DIR)/$(OUTPUT_DIR)/$(PARALLEL_LOG)
# 	@echo "main-test-parallel results saved in $(TST_DIR)/$(OUTPUT_DIR)/results/parallel-$(DS_NUM)-results.txt"
# 	@echo "main-test-parallel error logs saved in $(TST_DIR)/$(OUTPUT_DIR)/logs/parallel-$(DS_NUM)-errors.log"

	@echo "Running main-test-sequential with input from data/dataset-$(DS_NUM)..."
	@./$(SEQUENTIAL_EXEC) $(DS_NUM) > $(TST_DIR)/$(OUTPUT_DIR)/sequential-$(DS_NUM)-results.txt 2> $(TST_DIR)/$(OUTPUT_DIR)/logs/sequential-$(DS_NUM)-errors.log
	@echo "main-test-sequential results saved in $(TST_DIR)/$(OUTPUT_DIR)/results/sequential-$(DS_NUM)-results.txt"
	@echo "main-test-sequential error logs saved in $(TST_DIR)/$(OUTPUT_DIR)/logs/sequential-$(DS_NUM)-errors.log"

# Run parallel test only (parallel.cpp)
# par-test: $(PARALLEL_EXEC)
# 	@echo "Running ONLY main-test-parallel with input from data/dataset-$(DS_NUM)..."
# 	@./$(PARALLEL_EXEC) data/BBC-News-Training.csv  > $(TST_DIR)/$(OUTPUT_DIR)/$(PARALLEL_RES) 2> $(TST_DIR)/$(OUTPUT_DIR)/$(PARALLEL_LOG)
# 	@echo "main-test-parallel results saved in $(TST_DIR)/$(OUTPUT_DIR)/$(PARALLEL_RES)"
# 	@echo "main-test-parallel error logs saved in $(TST_DIR)/$(OUTPUT_DIR)/$(PARALLEL_LOG)"

# Run parallel test only (parallel.cpp)
seq-test: $(SEQUENTIAL_EXEC)
	@echo "Running ONLY main-test-sequential with input from data/dataset-$(DS_NUM)..."
	@./$(SEQUENTIAL_EXEC) $(DS_NUM)  > $(TST_DIR)/$(OUTPUT_DIR)/results/sequential-$(DS_NUM)-results.txt 2> $(TST_DIR)/$(OUTPUT_DIR)/logs/sequential-$(DS_NUM)-errors.log
	@echo "main-test-sequential results saved in $(TST_DIR)/$(OUTPUT_DIR)/results/sequential-$(DS_NUM)-results.txt 2>"
	@echo "main-test-sequential error logs saved in $(TST_DIR)/$(OUTPUT_DIR)/logs/sequential-$(DS_NUM)-errors.log"

2-test: $(PARALLEL_2_EXEC)	
	@echo "Running ONLY main-test-2threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_2_EXEC) $(DS_NUM) > tests/test-output/results/parallel-2-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-2-errors.log
	@echo "main-test-2threads results saved in tests/test-output/results/parallel-2-$(DS_NUM)-results.txt"
	@echo "main-test-2threads error logs saved in tests/test-output/logs/parallel-2-$(DS_NUM)-errors.log"


4-test: $(PARALLEL_4_EXEC)	
	@echo "Running ONLY main-test-4threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_4_EXEC) $(DS_NUM) > tests/test-output/results/parallel-4-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-4-errors.log
	@echo "main-test-4threads results saved in tests/test-output/results/parallel-4-$(DS_NUM)-results.txt"
	@echo "main-test-4threads error logs saved in tests/test-output/logs/parallel-4-$(DS_NUM)-errors.log"

8-test: $(PARALLEL_8_EXEC)	
	@echo "Running ONLY main-test-8threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_8_EXEC) $(DS_NUM) > tests/test-output/results/parallel-8-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-8-errors.log
	@echo "main-test-8threads results saved in tests/test-output/results/parallel-8-$(DS_NUM)-results.txt"
	@echo "main-test-8threads error logs saved in tests/test-output/logs/parallel-8-$(DS_NUM)-errors.log"

16-test: $(PARALLEL_16_EXEC)	
	@echo "Running ONLY main-test-16threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_16_EXEC) $(DS_NUM) > tests/test-output/results/parallel-16-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-16-errors.log
	@echo "main-test-16threads results saved in tests/test-output/results/parallel-16-$(DS_NUM)-results.txt"
	@echo "main-test-16threads error logs saved in tests/test-output/logs/parallel-16-$(DS_NUM)-errors.log"

32-test: $(PARALLEL_32_EXEC)	
	@echo "Running ONLY main-test-32threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_32_EXEC) $(DS_NUM) > tests/test-output/results/parallel-32-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-32-errors.log
	@echo "main-test-32threads results saved in tests/test-output/results/parallel-32-$(DS_NUM)-results.txt"
	@echo "main-test-32threads error logs saved in tests/test-output/logs/parallel-32-$(DS_NUM)-errors.log"

64-test: $(PARALLEL_64_EXEC)	
	@echo "Running ONLY main-test-64threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_64_EXEC) $(DS_NUM) > tests/test-output/results/parallel-64-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-64-errors.log
	@echo "main-test-64threads results saved in tests/test-output/results/parallel-64-$(DS_NUM)-results.txt"
	@echo "main-test-64threads error logs saved in tests/test-output/logs/parallel-64-$(DS_NUM)-errors.log"

128-test: $(PARALLEL_128_EXEC)	
	@echo "Running ONLY main-test-128threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_128_EXEC) $(DS_NUM) > tests/test-output/results/parallel-128-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-128-errors.log
	@echo "main-test-128threads results saved in tests/test-output/results/parallel-128-$(DS_NUM)-results.txt"
	@echo "main-test-128threads error logs saved in tests/test-output/logs/parallel-128-$(DS_NUM)-errors.log"

256-test: $(PARALLEL_256_EXEC)	
	@echo "Running ONLY main-test-256threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_256_EXEC) $(DS_NUM) > tests/test-output/results/parallel-256-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-256-errors.log
	@echo "main-test-256threads results saved in tests/test-output/results/parallel-256-$(DS_NUM)-results.txt"
	@echo "main-test-256threads error logs saved in tests/test-output/logs/parallel-256-$(DS_NUM)-errors.log"

512-test: $(PARALLEL_512_EXEC)	
	@echo "Running ONLY main-test-512threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_512_EXEC) $(DS_NUM) > tests/test-output/results/parallel-512-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-512-errors.log
	@echo "main-test-512threads results saved in tests/test-output/results/parallel-512-$(DS_NUM)-results.txt"
	@echo "main-test-512threads error logs saved in tests/test-output/logs/parallel-512-$(DS_NUM)-errors.log"

1024-test: $(PARALLEL_1024_EXEC)	
	@echo "Running ONLY main-test-1024threads.cpp with input from data/dataset-$(DS_NUM)..."
	@./$(PARALLEL_1024_EXEC) $(DS_NUM) > tests/test-output/results/parallel-1024-$(DS_NUM)-results.txt 2> tests/test-output/logs/parallel-1024-errors.log
	@echo "main-test-1024threads results saved in tests/test-output/results/parallel-1024-$(DS_NUM)-results.txt"
	@echo "main-test-1024threads error logs saved in tests/test-output/logs/parallel-1024-$(DS_NUM)-errors.log"

# Run non-optimized parallel test only (parallel.cpp)
# non-opt-par-test: $(NON_OPT_PARALLEL_EXEC)
# 	@echo "Running ONLY main-test-non-optimized-parallel with input from data/dataset-$(DS_NUM)..."
# 	@./$(NON_OPT_PARALLEL_EXEC) data/BBC-News-Training.csv  > $(TST_DIR)/$(OUTPUT_DIR)/results/non-opt-parallel-results.txt 2> $(TST_DIR)/$(OUTPUT_DIR)/logs/non-opt-parallel-errors.log
# 	@echo "main-test-non-opt-parallel results saved in $(TST_DIR)/$(OUTPUT_DIR)/results/non-opt-parallel-results.txt"
# 	@echo "main-test-non-opt-parallel error logs saved in $(TST_DIR)/$(OUTPUT_DIR)/logs/non-opt-parallel-errors.log"


# Files for zip to ignore
IGNORE_FILES= ./include/mlpack/* ./include/OleanderStemmingLibrary/* ./build/* ./data/* ./test_output/* README.md .vscode/* 

# Zip targets
zip-unused:
	zip -r kelton_project_cop4520.zip . -x "*.git*" "$(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/*" "*.DS_Store" "kelton_project_cop4520.zip"

zip:
	zip -r kelton_project_code.zip . -x "*.git*" "$(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/*" "*.DS_Store" "build/*" "data/*" ".vscode/*" "include/mlpack/*" "include/OleanderStemmingLibrary/*" "kelton_project_code.zip" "test_output/*" "test/*" "README.md"

zip-all:
	zip -r Parallel_TF-IDF_Classification.zip . -x "*.git*" "*.DS_Store" "test/build/*" ".gitignore" "*.zip"


# Clean build and test output
clean:
	rm -rf $(PARALLEL_EXEC) $(SEQUENTIAL_EXEC) $(NON_OPT_PARALLEL_EXEC) $(TST_DIR)/$(BUILD_DIR) 

.PHONY: all test clean zip zip-all
