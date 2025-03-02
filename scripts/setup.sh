#!/bin/bash

# Sets up the project directory for testing

# Directories for output testing
BUILD_TEST_DIR="./build"
BASE_TEST_DIR="./test-output"
COMP_TEST_DIR="./comparison"
SOLO_TEST_DIR="./solo"
RES_TEST_DIR="./results"
LOG_TEST_DIR="./logs"
PROCESSED_DATA_CSV_DIR="./processed-data-results"
GRAPHS_DIR="./graphs"
INCLUDE_DIR="./include"
STEM_LIB_DIR="$INCLUDE_DIR/OleanderStemmingLibrary"
ML_LIB_DIR="$INCLUDE_DIR/mlpack"
 
# welcome message
echo -e "\n"
echo "==========================================="
echo "    🚀 Welcome to the Parallel TF-IDF    "
echo "      Classification Setup Script! 🎉    "
echo "==========================================="
echo -e "\n"
echo "This script will create necessary directories"
echo "and check for required dependencies."
echo -e "\n"

sleep 3

echo -e "installing required dependencies...\n"

# Ensure required dependencies are installed
if [ -d "$STEM_LIB_DIR" ] && [ "$(ls -A "$STEM_LIB_DIR" 2>/dev/null)" ]; then
    echo "OleanderStemmingLibrary is already installed..."
else
    echo "OleanderStemmingLibrary not found. Cloning into $STEM_LIB_DIR..."

    # Remove the directory if it exists but is empty
    if [ -d "$STEM_LIB_DIR" ]; then
        rm -rf "$STEM_LIB_DIR"
    fi

    git clone https://github.com/Blake-Madden/OleanderStemmingLibrary.git "$STEM_LIB_DIR"

    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone OrleanderStemmingLibrary. Check your internet connection and try again."
        exit 1
    fi
fi

if [ -d "$ML_LIB_DIR" ] && [ "$(ls -A "$ML_LIB_DIR" 2>/dev/null)" ]; then
    echo "mlpack is already installed..."
else 
    echo "mlpack not found. Cloning into $ML_LIB_DIR..."

    # Remove the directory if it exists but is empty
    if [ -d "$ML_LIB_DIR" ]; then
        rm -rf "$ML_LIB_DIR"
    fi

    git clone https://github.com/mlpack/mlpack.git "$ML_LIB_DIR"

    if [ $? -ne 0 ]; then
        echo "Error: Failed to clone mlpack. Check your internet connection and try again."
        exit 1
    fi
fi

echo ""


ALL_TEST_DIRS=("$BASE_TEST_DIR" "$COMP_TEST_DIR" "$RES_TEST_DIR" "$LOG_TEST_DIR" "$SOLO_TEST_DIR" "$RES_TEST_DIR" "$LOG_TEST_DIR" "$PROCESSED_DATA_CSV_DIR" "$GRAPHS_DIR")

echo -e "Setting up testing directories...\n" 

# create build directory
mkdir -p "$BUILD_TEST_DIR"

# Check/Create directory for each 
# required directory for testing 
for DIR in "${ALL_TEST_DIRS[@]}"; do
    if [ ! -d "$DIR" ]; then
        echo "Creating directory: $DIR\n"
        mkdir -p "$DIR"

        if [ "$DIR" == "$GRAPHS_DIR" ]; then
            echo -e
        fi
    fi

    if [ "$DIR" == "$LOG_TEST_DIR" ]; then
        cd ".."  
    elif [ "$DIR" != "$RES_TEST_DIR" ] && [ "$DIR" != "$PROCESSED_DATA_CSV_DIR" ]; then
        cd "$DIR" 
    fi
done


# create DESCRIPTION file for test-output
cat > test-output/README.md <<EOF
### This directory stores output files generated during testing, graphs, and logs.

- processed-data-results/: Contains CSV 2 files with test results, 1. parallel tests, 2. sequential tests.
- graphs/: Stores generated graphs comparing sequential vs parallel performance.
- logs/: Holds logs related to test runs.

This directory is cleaned up by cleanup.sh and will delete all data, unless specified.
EOF


echo "Setup Complete!"