def compare_files(file1_path, file2_path, output_path):
    try:
        with open(file1_path, 'r') as file1, open(file2_path, 'r') as file2:
            file1_lines = file1.readlines()
            file2_lines = file2.readlines()

        with open(output_path, 'w') as output_file:
            output_file.write(f'Differences between {file1_path} and {file2_path}:')

            max_lines = max(len(file1_lines), len(file2_lines))
            for i in range(max_lines):
                line1 = file1_lines[i].strip() if i < len(file1_lines) else "[No Line]"
                line2 = file2_lines[i].strip() if i < len(file2_lines) else "[No Line]"

                if line1 != line2:
                    output_file.write(f"""Line {i + 1}:
File 1: {line1}
File 2: {line2}
""")

        print(f"Differences written to {output_path}")
    except FileNotFoundError as e:
        print(f"Error: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")


if __name__ == "__main__":
    # file1_path = input("Enter the path for the first file: ")
    # file2_path = input("Enter the path for the second file: ")
    # output_path = input("Enter the output file path: ")
    file1_path = "tests/test-output/lengthy/document-info.txt"
    file2_path = "tests/test-output/lengthy/document-info-par.txt"
    output_path = "tests/test-output/lengthy/diffy"
    compare_files(file1_path, file2_path, output_path)
