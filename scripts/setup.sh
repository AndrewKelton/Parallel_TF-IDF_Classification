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
 
# colors
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
MAGENTA='\033[0;35m'
BOLD='\033[1m'
NC='\033[0m'
RED='\033[0;31m'

# welcome message
echo -e "${CYAN}${BOLD}"
echo "  ██████╗  █████╗ ██████╗  █████╗ ██╗     ██╗     ███████╗██╗"
echo "  ██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║     ██║     ██╔════╝██║"
echo "  ██████╔╝███████║██████╔╝███████║██║     ██║     █████╗  ██║"
echo "  ██╔═══╝ ██╔══██║██╔══██╗██╔══██║██║     ██║     ██╔══╝  ██║"
echo "  ██║     ██║  ██║██║  ██║██║  ██║███████╗███████╗███████╗███████╗"
echo "  ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚══════╝"
echo -e "${NC}"
echo -e "${YELLOW}${BOLD}         🧠 TF-IDF Parallel Text Classifier — Setup 🚀${NC}"
echo -e "${MAGENTA}           ~ because sequential is so last century ~${NC}\n"
echo -e "${GREEN}  This script will:${NC}"
echo -e "   📦  Install the OleanderStemming library"
echo -e "   📁  Create all required output directories"
echo -e "   ✅  Get you ready to classify at ludicrous speed\n"

sleep 3

echo -e "${CYAN}  📦 Installing required dependencies...${NC}"

# Ensure required dependencies are installed
if [ -d "$STEM_LIB_DIR" ] && [ "$(ls -A "$STEM_LIB_DIR" 2>/dev/null)" ]; then
    echo -e "${GREEN}  ✅ OleanderStemmingLibrary is already installed!${NC}"
else
    echo -e "${YELLOW}  ⚠️  OleanderStemmingLibrary not found. Cloning into $STEM_LIB_DIR...${NC}"

    # Remove the directory if it exists but is empty
    if [ -d "$STEM_LIB_DIR" ]; then
        rm -rf "$STEM_LIB_DIR"
    fi

    git clone https://github.com/Blake-Madden/OleanderStemmingLibrary.git "$STEM_LIB_DIR"

    if [ $? -ne 0 ]; then
        echo -e "${RED}  ❌ Error: Failed to clone. Check your internet connection.${NC} and try again" 
        exit 1
    fi
fi
printf "\n"

# create testing directories if non existant
ALL_TEST_DIRS=( "$TEST_DIR" "$BASE_TEST_OUT_DIR" "$LENGTHY_TEST_DIR" "$BASE_RESULTS_DIR" "$BASE_LOGS_DIR" "$PROCESSED_DATA_CSV_DIR" "$GRAPHS_DIR")
echo -e "${CYAN}  📁 Setting up testing directories...${NC}"

mkdir -p "$BUILD_TEST_DIR" # create build directory

# Check/Create directory for each 
# required directory for testing 
for DIR in "${ALL_TEST_DIRS[@]}"; do
    if [ ! -d "$DIR" ]; then
        mkdir -p "$DIR"
        echo -e "${GREEN}  ✅ Created: $DIR${NC}"
    else 
        echo -e "${MAGENTA}  ✅ Already exists: $DIR${NC}"
    fi
done

printf "\n"

# create DESCRIPTION file for output
echo "
### This directory stores output files generated during testing, logs, and graphs.

- processed-data-results/: Contains 2 CSV files with test results, 1. parallel tests, 2. sequential tests.
- graphs/: Stores generated graphs comparing sequential vs parallel performance.
- .../results/: Holds output stdout from tests.
- .../logs/: Holds logs related to test runs.

This directory is cleaned up by cleanup.sh and will delete all data, unless specified.
" > $BASE_TEST_OUT_DIR/README.md

echo -e "${GREEN}${BOLD}  🎉 Setup Complete! Happy classifying! 🧠${NC}"