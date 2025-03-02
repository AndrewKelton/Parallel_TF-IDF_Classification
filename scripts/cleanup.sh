#!/bin/bash

# Cleans up the project directory removing all generated output, tests, build directories, and dependencies

echo "cleanup.sh will remove ALL testing outputs/directories, .zip files, build directories, and installed dependencies"

while true 
do
    read -p "Are you sure you want to continue (y/n)? " response
    
    if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
        break
    elif [ "$response" == "n" ] || [ "$response" == "N" ]; then
        exit 0
    fi
done

echo ""

# Clean (remove) up testing directories
BASE_TEST_DIR="./test-output"
BASE_PREV_TEST_DIR="./test_output"
BASE_PREV_PREV_TEST_DIR="./test"
BUILD_DIR="./build"

# Ensure all project level directories 'cleaned'
echo -e "Cleaning up testing directories...\n"
if [ -d $BUILD_DIR ]; then
    rm -r "$BUILD_DIR"
fi
if [ -d "$BASE_TEST_DIR" ]; then
    rm -r "$BASE_TEST_DIR"
    echo -e "Removed testing directories (test-output)...\n\n"
else
    echo -e "test-output does not exist...\n\n"
fi 


# remove installed dependencies
INCLUDE_DIR="./include"
ML_LIB_DIR="$INCLUDE_DIR/mlpack"
STEM_LIB_DIR="$INCLUDE_DIR/OleanderStemmingLibrary"

echo -e "Removing installed dependencies...\n"
if [ -d "$ML_LIB_DIR" ]; then
    rm -rf "$ML_LIB_DIR"
    echo -e "Successfully removed $ML_LIB_DIR"
fi
if [ -d "$STEM_LIB_DIR" ]; then
    rm -rf "$STEM_LIB_DIR"
    echo -e "Successfully removed $STEM_LIB_DIR"
fi


# Remove all .zip files in CWD
find . -type f -name "*.zip" -exec rm -f {} \;


# Use Makefile clean for build directory
make clean


echo "Cleanup complete!"