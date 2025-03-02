# !/bin/bash

NUM_ITERATIONS=100

for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 
done

python3 scripts/grapher.py