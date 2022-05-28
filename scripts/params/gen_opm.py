#! /usr/bin/env python3

import json

params = [
    {
        "id": 1,
        "path": "fb",
        "description": "Feedback",
        "shortDesc": "Feedback",
        "range": [0, 7],
        "default": 0
    },
    {
        "id": 2,
        "path": "conn",
        "description": "Connection",
        "shortDesc": "Connection",
        "range": [0, 7],
        "default": 0
    },
    {
        "id": 3,
        "path": "pms",
        "description": "Phase modulation sensitivity",
        "shortDesc": "PM",
        "range": [0, 7],
        "default": 0
    },
    {
        "id": 4,
        "path": "ams",
        "description": "Amplitude modulation sensitivity",
        "shortDesc": "AM",
        "range": [0, 7],
        "default": 0
    },
    {
        "id": 5,
        "path": "opsEnabled",
        "description": "Enabled operators",
        "shortDesc": "Operators",
        "range": [0, 0x0f],
        "default": 0x0f
    }
]


def op_params(op):
    i = ((op + 1) * 16)
    p_prefix = f"operators[{op}]"
    d_prefix = f"Operator {op+1}"
    ds_prefix = f"Op {op+1}"
    return [
        {
            "id": i + 0,
            "path": f"{p_prefix}.dt1",
            "description": f"{d_prefix} detune 1",
            "shortDesc": f"{ds_prefix} detune 1",
            "range": [0, 7],
            "default": 0
        },
        {
            "id": i + 1,
            "path": f"{p_prefix}.mul",
            "description": f"{d_prefix} frequency multiplier",
            "shortDesc": f"{ds_prefix} mult",
            "range": [0, 15],
            "default": 1
        },
        {
            "id": i + 2,
            "path": f"{p_prefix}.tl",
            "description": f"{d_prefix} total level",
            "shortDesc": f"{ds_prefix} level",
            "range": [0, 127],
            "default": 0 if op == 3 else 127,
            "invert": True
        },
        {
            "id": i + 3,
            "path": f"{p_prefix}.ks",
            "description": f"{d_prefix} key scaling",
            "shortDesc": f"{ds_prefix} KS",
            "range": [0, 3],
            "default": 0
        },
        {
            "id": i + 4,
            "path": f"{p_prefix}.ar",
            "description": f"{d_prefix} attack rate",
            "shortDesc": f"{ds_prefix} attack",
            "range": [0, 31],
            "default": 31
        },
        {
            "id": i + 5,
            "path": f"{p_prefix}.amsen",
            "description": f"{d_prefix} enable amplitude modulation",
            "shortDesc": f"{ds_prefix} AM",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 6,
            "path": f"{p_prefix}.d1r",
            "description": f"{d_prefix} decay rate 1",
            "shortDesc": f"{ds_prefix} D1R",
            "range": [0, 31],
            "default": 0
        },
        {
            "id": i + 7,
            "path": f"{p_prefix}.dt2",
            "description": f"{d_prefix} detune 2",
            "shortDesc": f"{ds_prefix} detune 2",
            "range": [0, 3],
            "default": 0
        },
        {
            "id": i + 8,
            "path": f"{p_prefix}.d2r",
            "description": f"{d_prefix} decay rate 2",
            "shortDesc": f"{ds_prefix} D2R",
            "range": [0, 31],
            "default": 0
        },
        {
            "id": i + 9,
            "path": f"{p_prefix}.d1l",
            "description": f"{d_prefix} decay level 1",
            "shortDesc": f"{ds_prefix} D1L",
            "range": [0, 15],
            "default": 0
        },
        {
            "id": i + 10,
            "path": f"{p_prefix}.rr",
            "description": f"{d_prefix} release rate",
            "shortDesc": f"{ds_prefix} RR",
            "range": [0, 15],
            "default": 15
        }
    ]


for op in range(0, 4):
    params.extend(op_params(op))

print(json.dumps(params, indent=2))
