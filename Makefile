# Makefile for tests

# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread \
		   -I/opt/homebrew/include -I/opt/homebrew/include/ \
		   -Iinclude -Iinclude/OleanderStemmingLibrary/src \
		   -Wdeprecated-declarations -Wno-unused-function \
		   -Wno-unused-variable -Wno-unused-parameter \
		   -Wno-catch-value -Wno-unused-value -Wno-sign-compare


# Project directories #
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build

# Test directories
OUTPUT_DIR = test-output
COMP_DIR = comparison
SOLO_DIR = solo

# Create necessary build directories
$(shell mkdir -p $(BUILD_DIR)/$(OBJ_DIR) $(BUILD_DIR))


# Common source files
COMMON_SOURCES = $(SRC_DIR)/count_vectorization.cpp \
				 $(SRC_DIR)/categories.cpp \
                 $(SRC_DIR)/document.cpp \
                 $(SRC_DIR)/preprocess.cpp \
                 $(SRC_DIR)/file_operations.cpp 

COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(OBJ_DIR)/%.o, $(COMMON_SOURCES))			 


# Parallel test source
PARALLEL_SOURCES = $(SRC_DIR)/main-test-parallel.cpp $(COMMON_SOURCES)
PARALLEL_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(OBJ_DIR)/%.o, $(PARALLEL_SOURCES))
PARALLEL_EXEC = $(BUILD_DIR)/main-test-parallel

# Non-Optimized Parallel test source
NON_OPT_PARALLEL_SOURCES = $(SRC_DIR)//main-test-non-optimized-parallel.cpp $(COMMON_SOURCES)
NON_OPT_PARALLEL_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(OBJ_DIR)/%.o, $(NON_OPT_PARALLEL_SOURCES))
NON_OPT_PARALLEL_EXEC = $(BUILD_DIR)/main-test-non-optimized-parallel

# Sequential test source 
SEQUENTIAL_SOURCES = $(SRC_DIR)/main-test-sequential.cpp $(COMMON_SOURCES)
SEQUENTIAL_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(OBJ_DIR)/%.o, $(SEQUENTIAL_SOURCES))
SEQUENTIAL_EXEC = $(BUILD_DIR)/main-test-sequential

# Txt to csv source (just used to convert txt to csv seperately for testing)
TO_CSV_SOURCES = $(SRC_DIR)/main-txt-to-csv.cpp $(COMMON_SOURCES)
TO_CSV_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/$(OBJ_DIR)/%.o, $(TO_CSV_SOURCES))
TO_CSV_EXEC = $(BUILD_DIR)/main-txt-to-csv


# Target executable names 
TARGET_PAR = main-par
TARGET_SEQ = main-seq
TARGET_NON = main-non-opt

# Build main-test and main-test-seq executables
all: $(PARALLEL_EXEC) $(SEQUENTIAL_EXEC) 

$(PARALLEL_EXEC): $(PARALLEL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(NON_OPT_PARALLEL_EXEC): $(NON_OPT_PARALLEL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SEQUENTIAL_EXEC): $(SEQUENTIAL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^	

# Executables with no automatic input
main-par: $(PARALLEL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main-seq: $(SEQUENTIAL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main-non-opt: $(NON_OPT_PARALLEL_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# txt to csv executable tester
$(TO_CSV_EXEC): $(TO_CSV_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^


# Compile object files
$(BUILD_DIR)/$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@



# Comparison tester, run both main-test-parallel.cpp & main-test-sequential.cpp
test: $(PARALLEL_EXEC) $(SEQUENTIAL_EXEC)
	@echo "Running main-test-parallel with input data/BBC-News-Training.csv..."
	@./$(PARALLEL_EXEC) data/BBC-News-Training.csv comparison > $(OUTPUT_DIR)/$(COMP_DIR)/results/main-test-parallel-results.txt 2> $(OUTPUT_DIR)/$(COMP_DIR)/logs/main-test-parallel-errors.log
	@echo "main-test-parallel results saved in $(OUTPUT_DIR)/$(COMP_DIR)/results/main-test-parallel-results.txt"
	@echo "main-test-parallel error logs saved in $(OUTPUT_DIR)/$(COMP_DIR)/logs/main-test-parallel-results.log"

	@echo "Running main-test-sequential with input data/BBC-News-Training.csv..."
	@./$(SEQUENTIAL_EXEC) data/BBC-News-Training.csv comparison > $(OUTPUT_DIR)/$(COMP_DIR)/results/main-test-sequential-results.txt 2> $(OUTPUT_DIR)/$(COMP_DIR)/logs/main-test-sequential-errors.log
	@echo "main-test-sequential results saved in $(OUTPUT_DIR)/$(COMP_DIR)/results/main-test-sequential-results.txt"
	@echo "main-test-sequential error logs saved in $(OUTPUT_DIR)/$(COMP_DIR)/logs/main-test-sequential-errors.log"

# Run parallel test only (main-test-parallel.cpp)
par-test: $(PARALLEL_EXEC)
	@echo "Running ONLY main-test-parallel with input data/BBC-News-Training.csv..."
	@./$(PARALLEL_EXEC) data/BBC-News-Training.csv solo > $(OUTPUT_DIR)/$(SOLO_DIR)/results/main-test-parallel-results-singleton.txt 2> $(OUTPUT_DIR)/$(SOLO_DIR)/logs/main-test-parallel-errors-singleton.log
	@echo "main-test-parallel results saved in $(OUTPUT_DIR)/$(SOLO_DIR)/results/main-test-parallel-results-singleton.txt"
	@echo "main-test-parallel error logs saved in $(OUTPUT_DIR)/$(SOLO_DIR)/logs/main-test-parallel-errors-singleton.log"

# Run parallel test only (main-test-parallel.cpp)
seq-test: $(SEQUENTIAL_EXEC)
	@echo "Running ONLY main-test-sequential  with input data/BBC-News-Training.csv..."
	@./$(SEQUENTIAL_EXEC) data/BBC-News-Training.csv solo > $(OUTPUT_DIR)/$(SOLO_DIR)/results/main-test-sequential-results-singleton.txt 2> $(OUTPUT_DIR)/$(SOLO_DIR)/logs/main-test-sequential-errors-singleton.log
	@echo "main-test-sequential results saved in $(OUTPUT_DIR)/$(SOLO_DIR)/results/main-test-sequential-results-singleton.txt"
	@echo "main-test-sequential error logs saved in $(OUTPUT_DIR)/$(SOLO_DIR)/logs/main-test-sequential-errors-singleton.log"

# Run non-optimized parallel test only (main-test-parallel.cpp)
non-opt-par-test: $(NON_OPT_PARALLEL_EXEC)
	@echo "Running ONLY main-test-non-optimized-parallel with input data/BBC-News-Training.csv..."
	@./$(NON_OPT_PARALLEL_EXEC) data/BBC-News-Training.csv solo > $(OUTPUT_DIR)/$(SOLO_DIR)/results/main-test-non-opt-parallel-results-singleton.txt 2> $(OUTPUT_DIR)/$(SOLO_DIR)/logs/main-test-non-opt-parallel-errors-singleton.log
	@echo "main-test-non-opt-parallel results saved in $(OUTPUT_DIR)/$(SOLO_DIR)/results/main-test-non-opt-parallel-results-singleton.txt"
	@echo "main-test-non-opt-parallel error logs saved in $(OUTPUT_DIR)/$(SOLO_DIR)/logs/main-test-non-opt-parallel-errors-singleton.log"


# Run main-txt-to-csv
txt-to-csv-test: $(TO_CSV_EXEC)
	@echo "Converting txt results to csv..."
	@./$(main-txt-to-csv)



# Files for zip to ignore
IGNORE_FILES= ./include/mlpack/* ./include/OleanderStemmingLibrary/* ./build/* ./data/* ./test/* ./test_output/* README.md .vscode/*

# Zip targets
zip-unused:
	zip -r kelton_project_cop4520.zip . -x "*.git*" "$(BUILD_DIR)/$(OBJ_DIR)/*" "*.DS_Store" "kelton_project_cop4520.zip"

zip:
	zip -r kelton_project_code.zip . -x "*.git*" "$(BUILD_DIR)/$(OBJ_DIR)/*" "*.DS_Store" "build/*" "data/*" ".vscode/*" "include/mlpack/*" "include/OleanderStemmingLibrary/*" "kelton_project_code.zip" "test_output/*" "test/*" "README.md"


# Clean build and test output
clean:
	rm -rf $(PARALLEL_EXEC) $(SEQUENTIAL_EXEC) $(NON_OPT_PARALLEL_EXEC) $(BUILD_DIR)/$(OBJ_DIR) $(BUILD_DIR)


.PHONY: all test clean zip zip_code 
