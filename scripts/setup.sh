# !/bin/bash

# Sets up the project directory for testing and installs required dependencies

set -e

ENV_FILE="scripts/.env"

if [ ! -f "$ENV_FILE" ]; then
    echo "Error: $ENV_FILE file not found!"
    exit 1
fi

source $ENV_FILE || { echo "Error: Failed to source $ENV_FILE"; exit 1; }
eval $(cat $ENV_FILE) || { echo "Error: Failed to evaluate $ENV_FILE"; exit 1; }
 

# welcome message
echo -e "\n"
echo "=============================================="
echo "      🚀 Welcome to the Parallel TF-IDF       "
echo "       Classification Setup Script! 🎉        "
echo "=============================================="
echo -e "\n"
echo "This script will create necessary directories,"
echo "check for required dependencies, and install"
echo "             them if necessary.              "
echo -e "\n"

sleep 3

echo -e "installing required dependencies..."

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

echo ""

while true 
do
    read -p "Would you like to setup test directories (y/n)? " response
    
    if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
        ALL_TEST_DIRS=( "$TEST_DIR" "$BASE_TEST_OUT_DIR" "$LENGTHY_TEST_DIR" "$COMP_TEST_DIR" "$SOLO_TEST_DIR" "$RES_TEST_DIR" "$LOG_TEST_DIR" "$PROCESSED_DATA_CSV_DIR" "$GRAPHS_DIR")

        echo -e "Setting up testing directories...\n" 

        # create build directory
        mkdir -p "$BUILD_TEST_DIR"

        # Check/Create directory for each 
        # required directory for testing 
        for DIR in "${ALL_TEST_DIRS[@]}"; do
            if [ ! -d "$DIR" ] && [ "$DIR" != "$RES_TEST_DIR" ] && [ "$DIR" != "$LOG_TEST_DIR" ]; then
                echo -e "Creating directory: $DIR"
                mkdir -p "$DIR"
            elif ( [ ! -d "$SOLO_TEST_DIR$DIR" ] || [ ! -d "$COMP_TEST_DIR$DIR" ] ) && [ "$DIR" == "$RES_TEST_DIR" ]; then
                echo -e "Creating directory: $SOLO_TEST_DIR$DIR"
                mkdir -p "$SOLO_TEST_DIR$DIR"
                echo -e "Creating directory: $COMP_TEST_DIR$DIR"
                mkdir -p "$COMP_TEST_DIR$DIR"
            elif ( [ ! -d "$SOLO_TEST_DIR$DIR" ] || [ ! -d "$COMP_TEST_DIR$DIR" ] ) && [ "$DIR" == "$LOG_TEST_DIR" ]; then
                echo -e "Creating directory: $SOLO_TEST_DIR$DIR"
                mkdir -p "$SOLO_TEST_DIR$DIR"
                echo -e "Creating directory: $COMP_TEST_DIR$DIR"
                mkdir -p "$COMP_TEST_DIR$DIR"
            fi
        done        
        echo -e "Test Directories Created...\n"
        break

    elif [ "$response" == "n" ] || [ "$response" == "N" ]; then
        break
    fi
done


# create DESCRIPTION file for test-output

    echo "
    ### This directory stores output files generated during testing, logs, and graphs.

    - processed-data-results/: Contains 2 CSV files with test results, 1. parallel tests, 2. sequential tests.
    - graphs/: Stores generated graphs comparing sequential vs parallel performance.
    - .../results/: Holds output stdout from tests.
    - .../logs/: Holds logs related to test runs.

    This directory is cleaned up by cleanup.sh and will delete all data, unless specified.
    " > $BASE_TEST_OUT_DIR/README.md


echo "Setup Complete!"