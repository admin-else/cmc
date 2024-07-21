import os
import jinja2

BASE_TEMPLATES_PATH = "templates"
ATTRIBUTE_FILE_SUFFIX = ".c.jinja"
env = jinja2.Environment()
types = {}

def get(type_name, attribute, data):
  if not types[type_name][attribute]:
    raise FileNotFoundError(f"Attribut {attribute} was not found in {type_name}.")
  template = env.from_string(types[type_name][attribute])
  return template.render(data)

def load_native_types():
  for type_name in os.scandir(BASE_TEMPLATES_PATH):
    for type_attribute in os.scandir(type_name):
      with open(type_attribute) as f:
        types.setdefault(type_name.name, {})
        types[type_name.name][type_attribute.name.removesuffix(ATTRIBUTE_FILE_SUFFIX)] = f.read()
        
load_native_types()
print(types)
