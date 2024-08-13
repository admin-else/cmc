import os
import re
import jinja2
import json
import shutil
import inspect

BASE_TEMPLATES_PATH = "generator/templates"
ATTRIBUTE_FILE_SUFFIX = ".c.jinja"
HEAD_PATH_TYPES = "include/cmc/type/"
CODE_PATH_TYPES = "src/type/"
env = jinja2.Environment()
types = {}
template_globals = {}
includes = []

def del_number_folders(path):
    for item in os.scandir(path):
        if item.is_dir and bool(re.match(r'^-?\d+(\.\d+)?$', item.name)):
            shutil.rmtree(item)

def include(h):
    global includes
    if h not in includes:
        includes.append(h)
    return ""

def render_includes():
    global includes
    return "\n".join([f"#include<{include}.h>"for include in includes])

def write(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(data)

def set_template_global(k: str, v):
    global template_globals
    template_globals[k] = v
    return ""


def json_loadf(fname):
    with open(fname, "r") as f:
        return json.load(f)

def sformat(format: str, *args):
    return format.format(*args)

# get by object
def get_bo(attribute, inp, *args, **kwargs):
    data = None
    if type(inp) is str:
        type_name = inp
    else:
        type_name = inp[0]
        data = inp[1]
    return get(attribute, type_name, data=data, *args, **kwargs)


def get(attribute, type_name, *args, **kwargs):
    contex = {"get": get, "get_bo": get_bo, "g": set_template_global, "include": include, "render_includes": render_includes, "sformat": lambda format, *args: format.format(*args), "break": input}
    data = kwargs
    data.update(contex)
    data.update(template_globals)

    for arg in args:
        data.update(arg)

    if type_name not in types:
        raise FileNotFoundError(f"Type {type_name} was not found.")
    if attribute not in types[type_name]:
        raise FileNotFoundError(f"Attribute {attribute} was not found in {type_name} data {data}.")
    template = env.from_string(types[type_name][attribute])
    
    pretty_data = {k: v for k, v in data.items() if not inspect.isfunction(v)}
    print(f"{type_name}.{attribute}({pretty_data})")


    return template.render(data)

def clear_get(attribute, type_name, *args, **kwargs):
    global includes, globals
    includes = []
    globals = {}
    return get(attribute, type_name, *args, **kwargs)

def load_native_types():
    for type_name in os.scandir(BASE_TEMPLATES_PATH):
        if type_name not in types:
            types[type_name.name] = {}
        for type_attribute in os.scandir(type_name):
            with open(type_attribute) as f:
                types[type_name.name][
                    type_attribute.name.removesuffix(ATTRIBUTE_FILE_SUFFIX)
                ] = f.read().strip()


def generate(name, rawdata, protocol_version):
    if rawdata == "native":
        if name not in types:
            print(f"native undfined! {name}")
            exit(1)
        print(name)
        return
    
    extra_data = {
        "data": rawdata,
        "name": name,
        "protvers": protocol_version,
        "t": f"cmc_{protocol_version}_{name}",
    }
    for attribute in types["type"]:
        if not attribute.startswith("export_"):
            continue
        export = attribute.removeprefix("export_")
        types.setdefault(name, {})
        types[name][export] = clear_get(attribute, "type", extra_data)
    # t short for type will be used VERY much so a short name is nice
    code = clear_get("code", "type", extra_data)
    header = clear_get("header", "type", extra_data)
    path = f"{protocol_version}/{name}"
    write(CODE_PATH_TYPES + path + ".c", code)
    write(HEAD_PATH_TYPES + path + ".h", header)

def main():
    del_number_folders(CODE_PATH_TYPES)
    del_number_folders(HEAD_PATH_TYPES)

    protocol_version = 765  # TODO: parse this
    proto = json_loadf("generator/minecraft-data/data/pc/1.20.3/protocol.json")
    left_types: list = list(proto["types"])
    no_repeat = []
    
    i = 0
    while left_types[i]:
        t = left_types[i]
        i += 1
        try:
            generate(t, proto["types"][t], protocol_version)
        except FileNotFoundError as e:
            if t in no_repeat:
                raise e
            left_types.append(t)
            no_repeat.append(t)
            

load_native_types()
main()
