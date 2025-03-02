# !/bin/bash

S_DIR="./scripts"

"$S_DIR/setup.sh"
"$S_DIR/run.sh"

while true
do
    read -p "Would you like to cleanup (y/n)?" response
    if [ "$response" == "y" ] || [ "$response" == "Y" ]; then
        break
    elif [ "$response" == "n" ] || [ "$response" == "N" ]; then
        exit 0
    fi
done

"$S_DIR/cleanup.sh"