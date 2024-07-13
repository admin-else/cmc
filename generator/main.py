from pprint import pprint
import json

DATAPATHS_FILE = "minecraft-data/data/dataPaths.json"
PROTO_BASE_PATH = "minecraft-data/data/"
PROTO_SUFFIX = "/protocol.json"

def get_files():
  with open(DATAPATHS_FILE, "r") as f:
    datapaths = json.load(f)
  return {version: datapaths["pc"][version]["protocol"] for version in datapaths["pc"]}

def get_proto(path):
  with open(PROTO_BASE_PATH + path + PROTO_SUFFIX, "r") as f:
    return json.load(f)

def get_native_types():
  get_files()
