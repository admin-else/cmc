def parse_packet(line):
    if line.startswith("#") or line == "packet_name,mcpacket_expresion,mcpacket_naming_expresion":
        return
    packet_name, expresion, naming_expresion = line.split(",")
    print(packet_name, expresion, naming_expresion)
    

with open("packets.csv", "r") as f:
    raw = f.read()

packets = raw.split("\n")
for packet in packets:
    parse_packet(packet)