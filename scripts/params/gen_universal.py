#! /usr/bin/env python3

import json

PARAM_MAP_PARAM_STEP = 0x10
LFO_PARAM_STEP = 0x10

NUM_PARAM_MAPS = 4
NUM_LFOS = 2

params = [
    {
        "id": 0x2000,
        "path": "polyMode",
        "description": "Polyphony mode",
        "shortDesc": "Poly mode",
        "range": [0, 4],
        "default": 0
    },
    {
        "id": 0x2001,
        "path": "fixedNoteNum",
        "description": "Fixed note number",
        "shortDesc": "Note num",
        "range": [0, 127],
        "default": 0
    },
    {
        "id": 0x2002,
        "path": "pitchOffset",
        "description": "Pitch offset",
        "shortDesc": "Pitch offset",
        "range": [0, 16383],
        "default": 0x2000,
        "zeroOffset": 0x2000
    },
    {
        "id": 0x2003,
        "path": "glideDurationMS",
        "description": "Glide duration (ms)",
        "shortDesc": "Glide",
        "range": [0, 16383],
        "default": 0
    }
]


def map_params(m):
    id_base = (m * PARAM_MAP_PARAM_STEP) + 0x3000
    p_prefix = f"paramMaps[{m}]"
    d_prefix = f"Map {m+1}"
    return [
        {
            "id": id_base + 0,
            "path": f"{p_prefix}.src",
            "description": f"{d_prefix} source",
            "shortDesc": f"{d_prefix} src",
            "range": [0, 0xff],
            "default": 0xff
        },
        {
            "id": id_base + 1,
            "path": f"{p_prefix}.dest",
            "description": f"{d_prefix} destination",
            "shortDesc": f"{d_prefix} dest",
            "range": [0, 0x3fff],
            "default": 0
        },
        {
            "id": id_base + 2,
            "path": f"{p_prefix}.adjustAmount",
            "description": f"{d_prefix} adjustment amount",
            "shortDesc": f"{d_prefix} amount",
            "range": [0, 0x3fff],
            "default": 0x2000,
            "zeroOffset": 0x2000
        }
    ]


def lfo_params(l):
    id_base = (l * LFO_PARAM_STEP) + 0x3100
    p_prefix = f"lfoParams[{l}]"
    d_prefix = f"LFO {l+1}"
    return [
        {
            "id": id_base + 0,
            "path": f"{p_prefix}.wave",
            "description": f"{d_prefix} wave",
            "shortDesc": f"{d_prefix} wave",
            "range": [0, 4],
            "default": 0
        },
        {
            "id": id_base + 1,
            "path": f"{p_prefix}.periodMS",
            "description": f"{d_prefix} period (ms)",
            "shortDesc": f"{d_prefix} period",
            "range": [0, 0x3fff],
            "default": 0
        },
        {
            "id": id_base + 2,
            "path": f"{p_prefix}.sync",
            "description": f"{d_prefix} sync",
            "shortDesc": f"{d_prefix} sync",
            "range": [0, 1],
            "default": 0
        },
        {
            "id": id_base + 3,
            "path": f"{p_prefix}.oneShot",
            "description": f"{d_prefix} one shot",
            "shortDesc": f"{d_prefix} one shot",
            "range": [0, 1],
            "default": 0
        }
    ]


for m in range(0, NUM_PARAM_MAPS):
    params.extend(map_params(m))

for l in range(0, NUM_LFOS):
    params.extend(lfo_params(l))

print(json.dumps(params, indent=2))
