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
    # File names
    input_filename = "data/unknown_text.txt"  # Replace with your actual input file
    output_filename = "data/unknown_text_fix.txt"
    removed_words_filename = "data/correct_unknown.txt"

    # Process the file
    process_file(input_filename, output_filename, removed_words_filename)

    print("Processing complete. Check output.txt and removed_words.txt")