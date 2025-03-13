#!/bin/bash

# Cleans up the project directory removing all generated output, tests, build directories, and dependencies

set -e
ENV_FILE="scripts/.env"

if [ ! -f "$ENV_FILE" ]; then
    echo "Error: $ENV_FILE file not found!"
    exit 1
fi

source $ENV_FILE || { echo "Error: Failed to source $ENV_FILE"; exit 1; }
eval $(cat $ENV_FILE) || { echo "Error: Failed to evaluate $ENV_FILE"; exit 1; }


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

# Ensure all project level directories 'cleaned'
if [ -d "$BASE_TEST_OUT_DIR" ]; then
    rm -r "$BASE_TEST_OUT_DIR"
    echo -e "Removed test output directories...\n\n"
fi 

if [ -d "$DOC_DIR/html" ]; then
    rm -r "$DOC_DIR/html"
    echo -e "Removed $DOC_DIR/html...\n\n"
fi
if [ -d "$DOC_DIR/latex" ]; then
    rm -r "$DOC_DIR/latex"
    echo -e "Removed $DOC_DIR/latex...\n\n"
fi

# remove installed dependencies
echo -e "Removing installed dependencies...\n"
if [ -d "$STEM_LIB_DIR" ]; then
    rm -rf "$STEM_LIB_DIR"
    echo -e "Successfully removed $STEM_LIB_DIR"
fi

# Makefile to clean up
make clean

# Remove all .zip files in CWD
find . -type f -name "*.zip" -exec rm -f {} \;


# all done
echo "Cleanup complete!"