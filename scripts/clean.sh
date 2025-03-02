#!/bin/bash

# Clean (remove) up testing directories
BASE_TEST_DIR="./test-output"
echo "Cleaning up testing directories..."

if [ -d "$BASE_TEST_DIR" ]; then
    rm -r "$BASE_TEST_DIR"
    echo "Removed testing directories..."
else 
    echo "No directories to clean up..."
fi 

make clean