#!/usr/bin/bash
cd $(dirname $0)/mcpacket-codegen
python3 codegen.py
cp ./Packets.* ../src