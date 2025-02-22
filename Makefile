# Define the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread \
		   -I/opt/homebrew/include -I/opt/homebrew/include/ \
		   -Iinclude -Iinclude/OleanderStemmingLibrary/src \
		   -Wdeprecated-declarations -Wno-unused-function \
		   -Wno-unused-variable -Wno-unused-parameter

# Project directories
SRC_DIR = src
OBJ_DIR = obj
BUILD_DIR = build
OUTPUT_DIR = test_output

# Create necessary directories
$(shell mkdir -p $(OBJ_DIR) $(BUILD_DIR) $(OUTPUT_DIR) $(OUTPUT_DIR)/logs $(OUTPUT_DIR)/results)

# Common source files
COMMON_SOURCES = $(SRC_DIR)/count_vectorization.cpp \
				 $(SRC_DIR)/categories.cpp \
                 $(SRC_DIR)/document.cpp \
                 $(SRC_DIR)/preprocess.cpp \
                 $(SRC_DIR)/file_operations.cpp 

COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(COMMON_SOURCES))			 

# Main test source (existing)
MAINTEST_SOURCES = $(SRC_DIR)/main-test-parallel.cpp $(COMMON_SOURCES)
MAINTEST_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAINTEST_SOURCES))
MAINTEST_EXEC = $(BUILD_DIR)/main-test-parallel

# Sequential test source (new)
MAINTESTSEQ_SOURCES = $(SRC_DIR)/main-test-sequential.cpp $(COMMON_SOURCES)
MAINTESTSEQ_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAINTESTSEQ_SOURCES))
MAINTESTSEQ_EXEC = $(BUILD_DIR)/main-test-sequential

# Build main-test and main-test-seq executables
all: $(MAINTEST_EXEC) $(MAINTESTSEQ_EXEC)

$(MAINTEST_EXEC): $(MAINTEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAINTESTSEQ_EXEC): $(MAINTESTSEQ_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run tests and store results in the test_output folder for both tests
test: $(MAINTEST_EXEC) $(MAINTESTSEQ_EXEC)
	@echo "Running main-test with input data/bbc-text.csv..."
	@./$(MAINTEST_EXEC) data/bbc-text.csv > $(OUTPUT_DIR)/results/main_test_results.txt 2> $(OUTPUT_DIR)/logs/main_test_errors.log
	@echo "main-test results saved in $(OUTPUT_DIR)/results/main_test_results.txt"
	@echo "main-test error logs saved in $(OUTPUT_DIR)/logs/main_test_errors.log"

	@echo "Running main-test-seq with input data/bbc-text.csv..."
	@./$(MAINTESTSEQ_EXEC) data/bbc-text.csv > $(OUTPUT_DIR)/results/main_test_seq_results.txt 2> $(OUTPUT_DIR)/logs/main_test_seq_errors.log
	@echo "main-test-seq results saved in $(OUTPUT_DIR)/results/main_test_seq_results.txt"
	@echo "main-test-seq error logs saved in $(OUTPUT_DIR)/logs/main_test_seq_errors.log"

# Clean build and test output
clean:
	rm -rf $(MAINTEST_EXEC) $(MAINTESTSEQ_EXEC) $(OBJ_DIR) $(BUILD_DIR)

# Zip targets
zip:
	zip -r kelton_project_cop4520.zip . -x "*.git*" "$(OBJ_DIR)/*" "*.DS_Store" "kelton_project_cop4520.zip"

zip_code:
	zip -r kelton_project_code.zip . -x "*.git*" "$(OBJ_DIR)/*" "*.DS_Store" "build/*" "data/*" ".vscode/*" "include/mlpack/*" "include/OleanderStemmingLibrary/*" "kelton_project_code.zip"

.PHONY: all test clean zip
