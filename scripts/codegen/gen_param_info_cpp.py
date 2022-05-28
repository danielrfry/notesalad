#! /usr/bin/env python3

import argparse
import json

parser = argparse.ArgumentParser(
    description='Generate C++ parameter info file')
parser.add_argument('json_file_path', metavar='JSON_FILE',
                    help='path to the JSON file containing parameter info')
parser.add_argument('cpp_file_path', metavar='CPP_FILE',
                    help='path to the generated C++ source code file')
parser.add_argument('-i', '--include', metavar='HEADER', nargs=1,
                    help='headers to include', action='append')
parser.add_argument('-n', '--var-name', metavar='NAME',
                    nargs=1, help='name of parameter info variable', default=['PARAMS'])

args = parser.parse_args()

with open(args.json_file_path, 'rb') as input_file:
    params_info = json.load(input_file)

with open(args.cpp_file_path, 'w', encoding='utf-8') as output:
    if args.include is not None:
        for include in args.include:
            output.write(f'#include "{include[0]}"\n')
        output.write("\n")

    output.write(f'const ParamInfo {args.var_name[0]}_ITEMS[] = {{\n')

    for param_info in params_info:
        param_id = param_info['id']
        param_default = param_info['default']
        param_desc = param_info['shortDesc']
        output.write(
            f'    {{ 0x{param_id:04x}, {param_default} }}, // {param_desc}\n')

    output.write('};\n')
    output.write('\n')

    output.write(
        f'const ParamInfoList {args.var_name[0]} = {{ {len(params_info)}, {args.var_name[0]}_ITEMS }};\n')
