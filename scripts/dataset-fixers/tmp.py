# tmp.py
# Splits testing data with correct category info into
# two files: 'tests/data/unknown_text_fix.txt' & 'tests/data/correct_unknown.txt'.
# * 'tests/data/correct_unknown.txt' contains the correct categories (1 category per line).
# * 'tests/data/unknown_text_fix.txt' contains the raw text data (1 article per line).

def process_file(input_file, output_file, removed_words_file):
    with open(input_file, 'r', encoding='utf-8') as infile, \
         open(output_file, 'w', encoding='utf-8') as outfile, \
         open(removed_words_file, 'w', encoding='utf-8') as removed_file:

        for line in infile:
            parts = line.split(',', 1)  # Split at the first comma
            if len(parts) == 2:
                removed_word, remaining_text = parts[0].strip(), parts[1].strip()
                removed_file.write(removed_word + '\n')
                outfile.write(remaining_text + '\n')

if __name__ == '__main__':
    
    input_filename = "tests/data/dataset-3/unknown_text_unfixed.txt" 
    output_filename = "tests/data/dataset-3/testing-data.txt"
    removed_words_filename = "tests/data/dataset-3/testing-correct-data.txt"

    process_file(input_filename, output_filename, removed_words_filename)

    print("Processing complete. Check output.txt and removed_words.txt")