# !/bin/bash

NUM_ITERATIONS=10

# Speedup tests
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 4-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 8-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 16-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 32-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 64-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 128-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 256-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 512-test
done
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make 1024-test
done

# Sequential test
for ((i=1; i<=$NUM_ITERATIONS; i++)); do
    make seq-test
done
