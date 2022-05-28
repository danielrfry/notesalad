#! /usr/bin/env python3

import json

params = [
    {
        "id": 0,
        "path": "bo",
        "description": "Basic octave",
        "shortDesc": "Octave",
        "range": [0, 3],
        "default": 1
    },
    {
        "id": 1,
        "path": "lfo",
        "description": "LFO frequency",
        "shortDesc": "LFO freq",
        "range": [0, 3],
        "default": 0
    },
    {
        "id": 2,
        "path": "alg",
        "description": "Algorithm",
        "shortDesc": "Algorithm",
        "range": [0, 7],
        "default": 0
    }
]


def op_params(op):
    i = ((op + 1) * 32)
    p_prefix = f"operators[{op}]"
    d_prefix = f"Operator {op+1}"
    ds_prefix = f"Op {op+1}"
    return [
        {
            "id": i + 0,
            "path": f"{p_prefix}.sr",
            "description": f"{d_prefix} sustain rate",
            "shortDesc": f"{ds_prefix} sustain",
            "range": [0, 15],
            "default": 0
        },
        {
            "id": i + 1,
            "path": f"{p_prefix}.xof",
            "description": f"{d_prefix} ignore keyoff",
            "shortDesc": f"{ds_prefix} XOF",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 2,
            "path": f"{p_prefix}.ksr",
            "description": f"{d_prefix} key scale sensitivity",
            "shortDesc": f"{ds_prefix} KSR",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 3,
            "path": f"{p_prefix}.rr",
            "description": f"{d_prefix} release rate",
            "shortDesc": f"{ds_prefix} release",
            "range": [0, 15],
            "default": 15
        },
        {
            "id": i + 4,
            "path": f"{p_prefix}.dr",
            "description": f"{d_prefix} decay rate",
            "shortDesc": f"{ds_prefix} decay",
            "range": [0, 15],
            "default": 0
        },
        {
            "id": i + 5,
            "path": f"{p_prefix}.ar",
            "description": f"{d_prefix} attack rate",
            "shortDesc": f"{ds_prefix} attack",
            "range": [0, 15],
            "default": 15
        },
        {
            "id": i + 6,
            "path": f"{p_prefix}.sl",
            "description": f"{d_prefix} sustain level",
            "shortDesc": f"{ds_prefix} sus level",
            "range": [0, 15],
            "default": 15
        },
        {
            "id": i + 7,
            "path": f"{p_prefix}.tl",
            "description": f"{d_prefix} total level",
            "shortDesc": f"{ds_prefix} level",
            "range": [0, 63],
            "default": 0 if op == 1 else 63,
            "invert": True
        },
        {
            "id": i + 8,
            "path": f"{p_prefix}.ksl",
            "description": f"{d_prefix} key scaling level",
            "shortDesc": f"{ds_prefix} KSL",
            "range": [0, 3],
            "default": 0
        },
        {
            "id": i + 9,
            "path": f"{p_prefix}.dam",
            "description": f"{d_prefix} amplitude modulation depth",
            "shortDesc": f"{ds_prefix} AM depth",
            "range": [0, 3],
            "default": 0
        },
        {
            "id": i + 10,
            "path": f"{p_prefix}.eam",
            "description": f"{d_prefix} enable amplitude modulation",
            "shortDesc": f"{ds_prefix} AM enable",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 11,
            "path": f"{p_prefix}.dvb",
            "description": f"{d_prefix} vibrato depth",
            "shortDesc": f"{ds_prefix} vib depth",
            "range": [0, 3],
            "default": 0
        },
        {
            "id": i + 12,
            "path": f"{p_prefix}.evb",
            "description": f"{d_prefix} enable vibrato",
            "shortDesc": f"{ds_prefix} vib enable",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": i + 13,
            "path": f"{p_prefix}.multi",
            "description": f"{d_prefix} frequency multiplier",
            "shortDesc": f"{ds_prefix} mult",
            "range": [0, 15],
            "default": 1
        },
        {
            "id": i + 14,
            "path": f"{p_prefix}.dt",
            "description": f"{d_prefix} detune",
            "shortDesc": f"{ds_prefix} detune",
            "range": [0, 7],
            "default": 0
        },
        {
            "id": i + 15,
            "path": f"{p_prefix}.ws",
            "description": f"{d_prefix} wave shape",
            "shortDesc": f"{ds_prefix} wave",
            "range": [0, 31],
            "default": 0
        },
        {
            "id": i + 16,
            "path": f"{p_prefix}.fb",
            "description": f"{d_prefix} feedback",
            "shortDesc": f"{ds_prefix} feedback",
            "range": [0, 7],
            "default": 0
        }
    ]


for op in range(0, 4):
    params.extend(op_params(op))

print(json.dumps(params, indent=2))
