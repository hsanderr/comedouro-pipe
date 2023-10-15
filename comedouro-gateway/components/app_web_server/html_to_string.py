def process_html_file(input_file_path, output_file_path):
    try:
        with open(input_file_path, 'r') as input_file, open(output_file_path, 'w') as output_file:
            for line in input_file:
                # Remove leading and trailing white spaces
                line = line.strip()

                # Replace double quotes with backslashes and double quotes
                line = line.replace('"', r'\"')

                # Write the processed line to the output file
                output_file.write(line)

    except FileNotFoundError:
        print(f"File not found: {input_file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    input_file_path = input(
        "Enter the relative path of the HTML file: (./web_page.html) ")
    output_file_path = input(
        "Enter the relative path of the output file: (./output.txt) ")

    if input_file_path == '':
        input_file_path = './web_page.html'

    if output_file_path == '':
        output_file_path = './output.txt'

    process_html_file(input_file_path, output_file_path)
