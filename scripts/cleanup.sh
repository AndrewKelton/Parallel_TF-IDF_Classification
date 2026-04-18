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

# colors
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
MAGENTA='\033[0;35m'
RED='\033[0;31m'
BOLD='\033[1m'
NC='\033[0m'

echo -e "${RED}${BOLD}"
echo "   ██████╗██╗     ███████╗ █████╗ ███╗   ██╗██╗   ██╗██████╗ "
echo "  ██╔════╝██║     ██╔════╝██╔══██╗████╗  ██║██║   ██║██╔══██╗"
echo "  ██║     ██║     █████╗  ███████║██╔██╗ ██║██║   ██║██████╔╝"
echo "  ██║     ██║     ██╔══╝  ██╔══██║██║╚██╗██║██║   ██║██╔═══╝ "
echo "  ╚██████╗███████╗███████╗██║  ██║██║ ╚████║╚██████╔╝██║     "
echo "   ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝     "
echo -e "${NC}"
echo -e "${YELLOW}${BOLD}            🧹 Parallel TF-IDF — Cleanup Script 🗑️${NC}"
echo -e "${MAGENTA}         ~ out with the old, in with the... nothing ~${NC}\n"
echo -e "${RED}  ⚠️  This will remove ALL of the following:${NC}"
echo -e "   🗂️   Testing output directories"
echo -e "   📄  Generated docs (html/latex)"
echo -e "   📦  Installed dependencies"
echo -e "   🔨  Build artifacts"
echo -e "   🗜️   .zip files\n"

while true 
do
    read -p $'\e[1;33m  ❓ Are you sure you want to continue (y/n)? \e[0m' response
    
    if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
        break
    elif [ "$response" == "n" ] || [ "$response" == "N" ]; then
        echo -e "${GREEN}  👋 Cleanup cancelled. Your files are safe!${NC}"
        exit 0
    fi
done

echo ""

# Ensure all project level directories 'cleaned'
if [ -d "$BASE_TEST_OUT_DIR" ]; then
    rm -r "$BASE_TEST_OUT_DIR"
    echo -e "${GREEN}  ✅ Removed test output directories${NC}"
fi 

if [ -d "$DOC_DIR/html" ]; then
    rm -r "$DOC_DIR/html"
    echo -e "${GREEN}  ✅ Removed $DOC_DIR/html${NC}"
fi
if [ -d "$DOC_DIR/latex" ]; then
    rm -r "$DOC_DIR/latex"
    echo -e "${GREEN}  ✅ Removed $DOC_DIR/latex${NC}"
fi

# remove installed dependencies
echo -e "\n${CYAN}  📦 Removing installed dependencies...${NC}"
if [ -d "$STEM_LIB_DIR" ]; then
    rm -rf "$STEM_LIB_DIR"
    echo -e "${GREEN}  ✅ Removed $STEM_LIB_DIR${NC}"
fi

# Remove all .zip files in CWD
echo -e "\n${CYAN}  🗜️  Removing .zip files...${NC}"
find . -type f -name "*.zip" -exec rm -f {} \;

# all done
echo -e "\n${GREEN}${BOLD}  🎉 Cleanup complete! Fresh start achieved. 🧼${NC}\n"