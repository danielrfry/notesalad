#! /usr/bin/env python3

import json

params = [
    {
        "id": 0,
        "path": "is4Op",
        "description": "4-op mode",
        "shortDesc": "4-op mode",
        "range": [0, 1],
        "default": 0
    },
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
        "range": [0, 3],
        "default": 0
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
            "path": f"{p_prefix}.am",
            "description": f"{d_prefix} enable amplitude modulation",
            "shortDesc": f"{ds_prefix} AM",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 1,
            "path": f"{p_prefix}.vib",
            "description": f"{d_prefix} enable vibrato",
            "shortDesc": f"{ds_prefix} VIB",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 2,
            "path": f"{p_prefix}.egt",
            "description": f"{d_prefix} envelope type",
            "shortDesc": f"{ds_prefix} EGT",
            "range": [0, 1],
            "default": 1
        },
        {
            "id": i + 3,
            "path": f"{p_prefix}.ksr",
            "description": f"{d_prefix} key scale rate",
            "shortDesc": f"{ds_prefix} KSR",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 4,
            "path": f"{p_prefix}.mult",
            "description": f"{d_prefix} frequency multiplier",
            "shortDesc": f"{ds_prefix} MULT",
            "range": [0, 15],
            "default": 1
        },
        {
            "id": i + 5,
            "path": f"{p_prefix}.ksl",
            "description": f"{d_prefix} key scale level",
            "shortDesc": f"{ds_prefix} KSL",
            "range": [0, 3],
            "default": 0
        },
        {
            "id": i + 6,
            "path": f"{p_prefix}.tl",
            "description": f"{d_prefix} total level",
            "shortDesc": f"{ds_prefix} level",
            "range": [0, 63],
            "default": 0 if op in (1, 3) else 63,
            "invert": True
        },
        {
            "id": i + 7,
            "path": f"{p_prefix}.ar",
            "description": f"{d_prefix} attack rate",
            "shortDesc": f"{ds_prefix} attack",
            "range": [0, 15],
            "default": 15
        },
        {
            "id": i + 8,
            "path": f"{p_prefix}.dr",
            "description": f"{d_prefix} decay rate",
            "shortDesc": f"{ds_prefix} decay",
            "range": [0, 15],
            "default": 0
        },
        {
            "id": i + 9,
            "path": f"{p_prefix}.sl",
            "description": f"{d_prefix} sustain level",
            "shortDesc": f"{ds_prefix} sustain",
            "range": [0, 15],
            "default": 15
        },
        {
            "id": i + 10,
            "path": f"{p_prefix}.rr",
            "description": f"{d_prefix} release rate",
            "shortDesc": f"{ds_prefix} release",
            "range": [0, 15],
            "default": 15
        },
        {
            "id": i + 11,
            "path": f"{p_prefix}.ws",
            "description": f"{d_prefix} wave shape",
            "shortDesc": f"{ds_prefix} wave",
            "range": [0, 7],
            "default": 0
        }
    ]


for op in range(0, 4):
    params.extend(op_params(op))

print(json.dumps(params, indent=2))
