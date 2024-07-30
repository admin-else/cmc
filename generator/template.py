import os
import jinja2
import json

BASE_TEMPLATES_PATH = "templates"
ATTRIBUTE_FILE_SUFFIX = ".c.jinja"
env = jinja2.Environment()
types = {}
template_globals = {}


def set_template_global(k: str, v):
    global template_globals
    template_globals[k] = v


def json_loadf(fname):
    with open(fname, "r") as f:
        return json.load(f)


# get by object
def get_bo(attribute, data, *args, **kwargs):
    if type(data) is str:
        type_name = data
    else:
        type_name = data[0]
        # args += tuple(data[1])
    return get(attribute, type_name, *args, **kwargs)


def get(attribute, type_name, *args, **kwargs):
    contex = {"get": get, "get_bo": get_bo, "g": set_template_global}
    if type_name not in types:
        raise FileNotFoundError(f"Type {type_name} was not found.")
    if attribute not in types[type_name]:
        raise FileNotFoundError(f"Attribute {attribute} was not found in {type_name}.")
    template = env.from_string(types[type_name][attribute])

    data = kwargs
    data.update(contex)
    data.update(template_globals)

    for arg in args:
        data.update(arg)

    return template.render(data)


def load_native_types():
    for type_name in os.scandir(BASE_TEMPLATES_PATH):
        for type_attribute in os.scandir(type_name):
            with open(type_attribute) as f:
                types.setdefault(type_name.name, {})
                types[type_name.name][
                    type_attribute.name.removesuffix(ATTRIBUTE_FILE_SUFFIX)
                ] = f.read()


def main():
    protocol_version = 765  # TODO: parse this
    types = json_loadf("minecraft-data/data/pc/1.20.3/protocol.json")
    for name in types["types"]:
        rawdata = types["types"][name]
        if rawdata == "native":
            if name not in types:
                print(f"native undfined! {name}")
                exit(1)
        # t short for type will be used VERY much so a short name is nice
        d = get(
            "code",
            "type",
            {
                "data": rawdata,
                "name": name,
                "t": f"cmc_{protocol_version}_{name}",
            },
        )  # type: ignore
        print(d)
        # print(rawdata)


load_native_types()
print(types)
main()
