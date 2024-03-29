#! /bin/bash

set -x

SCRIPT_DIR="$(dirname $(readlink -f $0))"
cd "$SCRIPT_DIR"

PARAMINFO_DIR="../resources/params"
PY_RESOURCES_DIR="../python/notesalad/resources"

./params/gen_universal.py > "$PARAMINFO_DIR/universal.json"
./params/gen_opl.py > "$PARAMINFO_DIR/opl.json"
./params/gen_opm.py > "$PARAMINFO_DIR/opm.json"
./params/gen_sd1.py > "$PARAMINFO_DIR/sd1.json"

./codegen/gen_param_info_cpp.py -i ParamInfo.h -n UNIVERSAL_PARAMS_INFO "$PARAMINFO_DIR/universal.json" ../libnotesaladcore/src/MIDI/ParamInfo.g.cpp
./codegen/gen_param_info_cpp.py -i OPLParamInfo.h -n OPL_PARAMS_INFO "$PARAMINFO_DIR/opl.json" ../libnotesaladcore/src/MIDI/OPL/OPLParamInfo.g.cpp
./codegen/gen_param_info_cpp.py -i OPMParamInfo.h -n OPM_PARAMS_INFO "$PARAMINFO_DIR/opm.json" ../libnotesaladcore/src/MIDI/OPM/OPMParamInfo.g.cpp
./codegen/gen_param_info_cpp.py -i SD1ParamInfo.h -n SD1_PARAMS_INFO "$PARAMINFO_DIR/sd1.json" ../libnotesaladcore/src/MIDI/SD1/SD1ParamInfo.g.cpp

cp "$PARAMINFO_DIR"/*.json "$PY_RESOURCES_DIR"

./doc/gen_params_md.py 
