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
BASE_TEST_DIR="./tests/"
BASE_PREV_TEST_DIR="$(BASE_TEST_DIR)/test_output"
# BASE_PREV_PREV_TEST_DIR="./test"
BUILD_DIR="$(BASE_TEST_DIR)/build"

# Ensure all project level directories 'cleaned'
if [ -d "$BASE_PREV_TEST_DIR" ]; then
    rm -r "$BASE_PREV_TEST_DIR"
    echo -e "Removed test output directories...\n\n"
fi 

if [ id "./docs/html" ]; then
    rm -r "./docs/html"
    echo -e "Removed docs/html...\n\n"
fi
if [ -d "./docs/latex" ]; then
    rm -r "./docs/latex"
    echo -e "Removed docs/latex...\n\n"
fi


# remove installed dependencies
INCLUDE_DIR="./include"
STEM_LIB_DIR="$INCLUDE_DIR/OleanderStemmingLibrary"

echo -e "Removing installed dependencies...\n"
if [ -d "$STEM_LIB_DIR" ]; then
    rm -rf "$STEM_LIB_DIR"
    echo -e "Successfully removed $STEM_LIB_DIR"
fi

# Makefile to clean up
make clean

# Remove all .zip files in CWD
find . -type f -name "*.zip" -exec rm -f {} \;



echo "Cleanup complete!"