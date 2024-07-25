import os
import jinja2
import json

BASE_TEMPLATES_PATH = "templates"
ATTRIBUTE_FILE_SUFFIX = ".c.jinja"
env = jinja2.Environment()
types = {}

contex = {}


def get(type_name, attribute, *args, **kwargs):
    if not types[type_name][attribute]:
        raise FileNotFoundError(f"Attribute {attribute} was not found in {type_name}.")
    template = env.from_string(types[type_name][attribute] + ATTRIBUTE_FILE_SUFFIX)
    data = kwargs
    data.update(contex)
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
    with open("minecraft-data/data/pc/1.20.3/protocol.json", "r") as f:
        types = json.load(f)
        for name in types["types"]:
            rawdata = types["types"][name]
            if rawdata == "native":
                continue  # TODO: Check if type is in types array

            get("type", "code", rawdata)


load_native_types()
print(types)
main()
