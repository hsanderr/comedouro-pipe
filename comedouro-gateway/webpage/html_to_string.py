'''
Program to transform an HTML file into a compatible C string.
'''

# Imports to read directory files
from os import listdir
from os.path import isfile, join

post = '"\n\n#endif'


def process_html_file(input_file_path, output_file_path, define_name):
    try:
        with open(input_file_path, 'r') as input_file, open(output_file_path, 'w') as output_file:
            define_name_h = '{}_H'.format(define_name)
            pre = '#ifndef {}\n#define {}\n\n#define {} "'.format(
                define_name_h, define_name_h, define_name)
            output_file.write(pre)
            for line in input_file:
                # Remove leading and trailing white spaces
                line = line.strip()

                # Replace double quotes with backslashes and double quotes
                line = line.replace('"', r'\"')

                # Write the processed line to the output file
                output_file.write(line)
            output_file.write(post)

    except FileNotFoundError:
        print(f'File not found: {input_file_path}')
    except Exception as e:
        print(f'An error occurred: {e}')


if __name__ == '__main__':
    files = [f for f in listdir('.') if (
        isfile(join('.', f)) and f.endswith('.html'))]

    user_input = ''
    input_msg = 'Select a file:\n'

    for index, item in enumerate(files):
        input_msg += f'{index+1}) {item}\n'

    input_msg += '> '

    while user_input not in map(str, range(1, len(files) + 1)):
        user_input = input(input_msg)

    input_file = f'./{files[int(user_input) - 1]}'

    print('You selected: ' + files[int(user_input) - 1])

    output_file_path = input(
        'Enter the path of the output file: (../components/user_httpd/include/) ')

    if output_file_path == '':
        output_file_path = '../components/user_httpd/include/'

    output_file_name = input(
        'Enter the name of the output file: (output.h) ')

    if output_file_name == '':
        output_file_name = 'output.h'

    output_file = '{}{}'.format(output_file_path, output_file_name)

    define_name = input('Define name: (DEFINE_NAME_EXAMPLE) ')

    if define_name == '':
        define_name = 'DEFINE_NAME_EXAMPLE'

    process_html_file(input_file, output_file, define_name)
