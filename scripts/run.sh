# !/bin/bash

NUM_ITERATIONS=1

# make clean
make test

total_count=0
correct_count=0
parallel_count=0
sequential_count=0
parallel_correct=0
sequential_correct=0

for ((y=1; y <= NUM_ITERATIONS; y++)); do
    echo "-----------------------------------------------"
    echo "                    Test #$y                "
    echo "-----------------------------------------------"
    for ((i=1; i <=3; i++)); do
        echo ""
        echo "                   Dataset-$i                 "
        echo "* --- * --- * --- * ------- * --- * --- * --- *"
        for ((j=1; j <=1024; j*=2)); do
            printf "Testing w/%-4s Thread(s) " "$j"
            ((total_count++))

            if [ $j == 1 ]; then
                # ./test $i
                file="tests/output/results/sequential-$i-results.txt"
                ((sequential_count++))

                if ! grep -q "Vectorization" "$file" || ! grep -q "TF-IDF" "$file" || ! grep -q "Categories" "$file" || ! grep -q "Unknown Classification" "$file"; then
                    echo -e " Failed \033[0;31m❌\033[0m"
                else
                    echo -e " Passed \033[0;32m✅\033[0m"
                    ((sequential_correct++))
                    ((correct_count++))
                fi
            else 
                # ./test $i $j
                file="tests/output/results/parallel-$j-$i-results.txt"
                ((parallel_count++))

                if ! grep -q "Vectorization" "$file" || ! grep -q "TF-IDF" "$file" || ! grep -q "Categories" "$file" || ! grep -q "Unknown Classification" "$file"; then
                    echo -e " Failed \033[0;31m❌\033[0m"
                else
                    echo -e " Passed \033[0;32m✅\033[0m"
                    ((parallel_correct++))
                    ((correct_count++))
                fi
            fi  
        done
    done
    echo ""
done

echo "                Testing Report                 "
echo "-----------------------------------------------"
echo "Total Iterations $NUM_ITERATIONS"
printf "Passed %-8s total tests\n" "$correct_count/$total_count"
printf "Passed %-8s parallel tests\n" "$parallel_correct/$parallel_count"
printf "Passed %-8s sequential tests\n" "$sequential_correct/$sequential_count"
