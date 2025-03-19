# !/bin/bash

set -e
ENV_FILE="scripts/.env"

if [ ! -f "$ENV_FILE" ]; then
    echo "Error: $ENV_FILE file not found!"
    exit 1
fi

source $ENV_FILE || { echo "Error: Failed to source $ENV_FILE"; exit 1; }
eval $(cat $ENV_FILE) || { echo "Error: Failed to evaluate $ENV_FILE"; exit 1; }

"$SCRIPT_DIR/setup.sh"
"$SCRIPT_DIR/run.sh"

while true
do
    read -p "Would you like to cleanup (y/n)?" response
    if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
        break
    elif [ "$response" == "n" ] || [ "$response" == "N" ]; then
        exit 0
    fi
done

"$SCRIPT_DIR/cleanup.sh"