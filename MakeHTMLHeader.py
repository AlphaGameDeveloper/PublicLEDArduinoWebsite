import os
import sys

def escape_html(html_content):
    replacements = {
        '\\': '\\\\',
        '\"': '\\\"',
        '\n': '\\n',
        '\t': '\\t',
    }
    escaped_html = ''.join(replacements.get(c, c) for c in html_content)
    return escaped_html

def read_html(file_path):
    with open(file_path, 'r') as file:
        html_content = file.read()
    return html_content

def write_cpp_header(html_content, variable_name, header_guard, output_file):
    with open(output_file, 'w') as file:
        file.write(f'#ifndef {header_guard}\n')
        file.write(f'#define {header_guard}\n\n')
        file.write(f'const char {variable_name}[] = "{html_content}";\n\n')
        file.write(f'#endif // {header_guard}\n')

def main():
    if len(sys.argv) != 5:
        print("Usage: %s <variable_name> <html_file_path> <header_guard_value> <output_header_path>" % sys.argv[0])
        return

    variable_name = sys.argv[1]
    input_html_path = sys.argv[2]
    header_guard = sys.argv[3]
    output_header_path = sys.argv[4]

    if not os.path.isfile(input_html_path):
        print("The specified file does not exist.")
        return

    html_content = read_html(input_html_path)
    escaped_html = escape_html(html_content)
    write_cpp_header(escaped_html, variable_name, header_guard, output_header_path)
    print(f'C++ header file has been generated: {output_header_path}')

if __name__ == "__main__":
    main()
