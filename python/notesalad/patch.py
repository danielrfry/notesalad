import pydash
from .resources import PARAMS_UNIVERSAL


def encode_leb128(value):
    result = bytearray()
    while True:
        byte = value & 0x7f
        value >>= 7
        if value == 0:
            result.append(byte)
            break
        else:
            result.append(byte | 0x80)
    return result


def serialize(json_patch, json_params):
    result = bytearray()
    all_params = [*json_params, *PARAMS_UNIVERSAL]
    last_param_id = 0
    for param_info in all_params:
        param_value = pydash.get(json_patch, param_info['path'])
        if param_value is not None and param_value != param_info['default']:
            param_id = param_info['id']
            result.extend(encode_leb128(param_id - last_param_id))
            result.extend(encode_leb128(param_value))
            last_param_id = param_id
    return result
