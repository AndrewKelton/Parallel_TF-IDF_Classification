# !/bin/bash

NUM_ITERATIONS=10

for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make par-test
done

for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make seq-test
done
