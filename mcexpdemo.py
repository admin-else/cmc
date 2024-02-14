from quarry.types.buffer import Buffer1_7

def split_carefully(raw_exps):
    deepness = 0
    current_exp = ""
    exps = []
    for char in raw_exps:
        if char == "[":
            deepness += 1
        if char == "]":
            deepness -= 1
            if deepness < 0:
                raise ValueError(f"invalid exp \"{raw_exps}\" (closed nonexisted brace)")
        if char == ";" and deepness == 0:
            exps.append(current_exp)
            current_exp = ""
        else:
            current_exp += char
    if deepness != 0:
        raise ValueError(f"invalid exp \"{raw_exps}\" (didnt close brace)")
    
    if current_exp:
        exps.append(current_exp)
    return exps

def parse2(raw_exps, buff: Buffer1_7):
    exps = split_carefully(raw_exps)

    out = {}
    for exp in exps:
        exp_type, data = exp[0], exp[1:]
        if exp_type in "bB?hHiIlLqQfd": # see https://docs.python.org/3/library/struct.html
            out[data] = buff.unpack(exp_type)
        elif exp_type == "v":
            out[data] = buff.unpack_varint()
        elif exp_type == "s":
            out[data] = buff.unpack_string()
        elif exp_type == "p":
            out[data] = buff.unpack_position()
        elif exp_type == "n":
            out[data] = buff.unpack_nbt()
        elif exp_type == "j":
            out[data] = buff.unpack_json()
        elif exp_type == "a":
            out[data] = buff.unpack_byte_array()
        elif exp_type == "S":
            out[data] = buff.unpack_slot()
        elif exp_type == "m":
            out[data] = buff.unpack_entity_metadata()
        elif exp_type == "c":
            out[data] = buff.unpack_chat()
        elif exp_type == "A":
            count_key = data[data.find("]")+1:]
            array_exp = data[data.find("[")+1:data.find("]")]
            name = data[:data.find("[")]
            out[name] = []
            if count_key not in out:
                raise ValueError("exp gave nonexisted count key")
            for _ in range(out[count_key]):
                out[name].append(parse2(array_exp, buff))
        else:
            raise ValueError(f"unkown type \"{exp_type}\".")
    
    return out

def parse(exp, buff: Buffer1_7):
    exp = exp.replace(" ", "")
    if exp.find("#") != -1:
        exp = exp[:exp.find("#")]

    
    name, packet_id, exp = exp.split(";", maxsplit=2)
    if exp == "!":
        return {"name": name, "data": "custom function"}
    return {"name": name, "data": parse2(exp, buff)}


update_teams_exp = "update_teams,0x5E,steam_name,bmode,taction[create_team:cteam_display_name,bfriendly_flags,sname_tag_visiblility,scollision_rule,vteam_color,cteam_prefix,cteam_suffix,ventity_count,Aentities[sentity]entity_count;remove_team:;update_team:cteam_display_name,bfriendly_flags,sname_tag_visiblility,scollision_rule,vteam_color,cteam_prefix,cteam_suffix,ventity_count,Aentities[sentity]entity_count;add_entities:ventity_count,Aentities[sentity]ventity_count;remove_entities:ventity_count,Aentities[sentity]ventity_count]"
exp = "susypacket;0x00;vcount;Alol[vsus]count"
b = Buffer1_7()
print(parse(exp, Buffer1_7(b"".join([b.pack_varint(1), b.pack_varint(1)]))))