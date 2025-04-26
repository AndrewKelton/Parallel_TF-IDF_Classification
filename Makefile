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
OUTPUT_DIR = output

# Create necessary build directories
$(shell mkdir -p $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR) $(TST_DIR)/$(BUILD_DIR))


# Common source files
COMMON_SOURCES = $(SRC_DIR)/count_vectorization.cpp \
				 $(SRC_DIR)/categories.cpp \
                 $(SRC_DIR)/document.cpp \
                 $(SRC_DIR)/preprocess.cpp \
                 $(SRC_DIR)/file_operations.cpp \
				 $(SRC_DIR)/TFIDF.cpp 

# COMMON_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(COMMON_SOURCES))			 


# main.cpp test file
MAIN_SOURCES = $(TST_DIR)/src/main.cpp $(COMMON_SOURCES)
MAIN_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o, $(MAIN_SOURCES))
MAIN_EXEC = $(TST_DIR)/$(BUILD_DIR)/main


# executables
all: $(MAIN_EXEC)

test: $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(MAIN_EXEC): $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^


# compile object files
$(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(TST_DIR)/$(BUILD_DIR)/$(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

zip:
	zip -r Parallel_TF-IDF_Classification . -x "*.git*" "$(TST_DIR)/$(BUILD_DIR)"  "*.DS_Store" ".vscode/" "include/OleanderStemmingLibrary/" "venv"
# clean
clean:
	rm -rf $(MAIN_EXEC) $(TST_DIR)/$(BUILD_DIR) test main

.PHONY: all test clean