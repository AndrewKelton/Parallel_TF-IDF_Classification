import csv

input_file = "tests/data/bbc-news-data.csv"  # Change this to your actual file path
output_file = "tests/data/bbc-news-data-fixed.csv"

# Manually set the delimiter if known (modify this if `head -n 1` shows a different one)
KNOWN_DELIMITERS = ["\x1b", "|", ";", "\t"]  # Add other potential delimiters if necessary

# Detect the delimiter
with open(input_file, "r", encoding="utf-8-sig") as infile:
    sample = infile.read(1000)  # Read a small portion to analyze
    delimiter = next((d for d in KNOWN_DELIMITERS if d in sample), ",")

# Read and process the CSV with the detected delimiter
with open(input_file, "r", encoding="utf-8-sig") as infile, open(output_file, "w", encoding="utf-8", newline="") as outfile:
    reader = csv.reader(infile, delimiter=delimiter)
    headers = next(reader)

    # Normalize headers
    headers = [header.strip().lower() for header in headers]

    # Expected headers mapping
    try:
        header_index = {
            "category": headers.index("category"),
            "content": headers.index("content")
        }
    except ValueError:
        print("Error: Expected headers not found. Check delimiter or header formatting.")
        exit(1)

    writer = csv.writer(outfile)
    writer.writerow(["category", "text"])  # New header format

    # Process rows
    for row in reader:
        category = row[header_index["category"]].strip()
        content = row[header_index["content"]].strip().replace("\n", " ")  # Ensure single-line content
        writer.writerow([category, content])

print(f"CSV file reformatted using delimiter '{repr(delimiter)}' and saved as {output_file}")
