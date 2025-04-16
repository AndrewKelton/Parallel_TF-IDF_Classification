import csv
import random

input_file = "tests/data/dataset-3/training-data.csv"  # Change this to your actual input file
output_file = "tests/data/dataset-3/shuffled_output.csv"

# Read the CSV file
with open(input_file, "r", encoding="utf-8") as infile:
    reader = csv.reader(infile)
    header = next(reader)  # Read header
    rows = list(reader)  # Read the remaining rows

# Shuffle the rows randomly
random.shuffle(rows)

# Write the shuffled rows back to a new CSV file
with open(output_file, "w", encoding="utf-8", newline="") as outfile:
    writer = csv.writer(outfile)
    writer.writerow(header)  # Write header first
    writer.writerows(rows)  # Write shuffled rows

print(f"Shuffled CSV file saved as {output_file}")
