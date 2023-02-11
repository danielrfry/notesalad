import importlib.resources
import json


def load_json_resource(name):
    with importlib.resources.files(__package__).joinpath(name).open('rb') as json_file:
        return json.load(json_file)


PARAMS_OPL = load_json_resource('opl.json')
PARAMS_OPM = load_json_resource('opm.json')
PARAMS_SD1 = load_json_resource('sd1.json')
PARAMS_UNIVERSAL = load_json_resource('universal.json')
