#!/bin/bash

echo "cleanup.sh will remove ALL testing outputs, .zip files, and build directories"
# echo "Are you sure you want to continue? (y/n)"
# read continue

while true 
do
    read -p "Are you sure you want to continue (y/n)? " response
    
    if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
        break
    elif [ "$response" == "n" ] || [ "$response" == "N" ]; then
        exit 0
    fi
done

# Clean (remove) up testing directories
BASE_TEST_DIR="./test-output"
BASE_PREV_TEST_DIR="./test_output"
BASE_PREV_PREV_TEST_DIR="./test"

# Ensure all project level directories 'cleaned'
echo "Cleaning up testing directories..."
if [ -d "$BASE_TEST_DIR" ]; then
    rm -r "$BASE_TEST_DIR"
    echo "Removed testing directories (test-output)..."
else
    echo "test-output does not exist..."
fi 

# Ensure all test directories are removed including prior names
echo "removing previous version testing directories"
if [ -d "$BASE_PREV_TEST_DIR" ]; then
    rm -r "$BASE_PREV_TEST_DIR"
    echo "removed $BASE_PREV_TEST_DIR"
fi
if [ -d "$BASE_PREV_PREV_TEST_DIR" ]; then
    rm -r "$BASE_PREV_PREV_TEST_DIR"
    echo "removed $BASE_PREV_PREV_TEST_DIR"
fi

# Remove all .zip files in CWD
find . -type f -name "*.zip" -exec rm -f {} \;

# Use Makefile clean for build directory
make clean