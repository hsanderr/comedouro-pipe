'''
Program to transform an HTML file into a compatible C string.
'''

# Imports to read directory files
from os import listdir
from os.path import isfile, join


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
    files = [f for f in listdir('.') if isfile(join('.', f))]

    user_input = ''
    input_msg = 'Select a file:\n'

    for index, item in enumerate(files):
        input_msg += f'{index+1}) {item}\n'

    input_msg += '> '

    while user_input not in map(str, range(1, len(files) + 1)):
        user_input = input(input_msg)

    input_file = f'./{files[int(user_input) - 1]}'

    print('You selected: ' + files[int(user_input) - 1])

    # input_file_path = input(
    #     "Enter the relative path of the HTML file: (./web_page.html) ")
    output_file = input(
        "Enter the path of the output file: (./output.txt) ")

    # if input_file_path == '':
    #     input_file_path = './web_page.html'

    if output_file == '':
        output_file = './output.txt'

    process_html_file(input_file, output_file)
