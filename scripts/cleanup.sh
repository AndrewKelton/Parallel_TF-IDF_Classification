#!/bin/bash

# Clean (remove) up testing directories
BASE_TEST_DIR="./test-output"
BUILD_DIR="./build"
echo "Cleaning up testing directories..."

make clean

# Ensure all project level directories 'cleaned'
if [ -d "$BASE_TEST_DIR" ]; then
    rm -r "$BASE_TEST_DIR"
    echo "Removed testing directories..."
else if [ -d "$BUILD_DIR" ]; then
    rm -r "$BUILD_DIR"
    echo "Removed build directories..."
else
    echo "No directories to clean up..."
fi 