#! /usr/bin/env python3

import json
import os.path


def format_table_row(row, col_widths):
    fmt_row = []
    for col_index in range(0, len(row)):
        fmt_row.append(row[col_index] + ' ' *
                       (col_widths[col_index] - len(row[col_index])))
    return '| ' + ' | '.join(fmt_row) + ' |'


def format_table(rows, header=None):
    header = [] if header is None else header
    table_lines = []
    col_widths = [len(x) for x in header]
    for row in rows:
        for (col_index, col) in enumerate(row):
            if col_index >= len(col_widths):
                col_widths.append(0)
            col_widths[col_index] = max(len(col), col_widths[col_index])
    if len(header) > 0:
        table_lines.append(format_table_row(header, col_widths))
        table_lines.append(
            '| ' + ' | '.join(['-' * w for w in col_widths]) + ' |')
    for row in rows:
        table_lines.append(format_table_row(row, col_widths))
    return table_lines


def gen_params_table(params):
    rows = []
    for param in params:
        param_id = param['id']
        desc = param['description']
        v_min, v_max = param['range']
        row = [f'{param_id:d}', f'0x{param_id:04x}',
               desc, f'{v_min:d}-{v_max:d}']
        rows.append(row)
    return format_table(rows, ['NRPN', 'Hex', 'Parameter', 'Range'])


base_dir = os.path.join(os.path.dirname(__file__), '..', '..')
chips = [
    {
        'name': 'OPL',
        'params_file': 'resources/params/opl.json',
        'md_file': 'doc/opl-params.md'

    },
    {
        'name': 'OPM',
        'params_file': 'resources/params/opm.json',
        'md_file': 'doc/opm-params.md'
    },
    {
        'name': 'SD-1',
        'params_file': 'resources/params/sd1.json',
        'md_file': 'doc/sd1-params.md'
    }
]

json_file_path = os.path.join(base_dir, 'resources/params/universal.json')
with open(json_file_path, 'rb') as json_file:
    universal_params_data = json.load(json_file)

for chip in chips:
    json_file_path = os.path.join(base_dir, chip['params_file'])
    with open(json_file_path, 'rb') as json_file:
        chip_params_data = json.load(json_file)
    lines = []
    lines.append(f'# {chip["name"]} Parameters')
    lines.append('')
    lines.extend(gen_params_table(chip_params_data))
    lines.append('')
    lines.append('# Universal Parameters')
    lines.append('')
    lines.extend(gen_params_table(universal_params_data))
    md_file_path = os.path.join(base_dir, chip['md_file'])
    with open(md_file_path, 'w', encoding='utf-8') as md_file:
        md_file.writelines([line + '\n' for line in lines])
