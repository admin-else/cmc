from codegen import type_map


exp = "samogus;vcount;Alol[vsus;sdada;Anigga[vsudd;sdadauu]]count"

def careful_split(exp):
    out = []
    curr_str = ""
    deepness = 0
    for c in exp:
        if c == ";" and deepness == 0:
            out.append(curr_str)
            curr_str = ""
        else:
            curr_str += c
        
        if c == "[":
            deepness += 1
        elif c == "]":
            deepness -= 1
            if deepness < 0:
                raise ValueError("closed nonexistent array") 
    out.append(curr_str)
    return out

def free_method_content(exp, tofree, deepness, packet_name):
    code = ""
    for sym in careful_split(exp):
        if type_map[sym[0]][2]:
            exp_type = sym[0]
            exp_data = sym[1:]
            if exp_type == "A":
                array_exp = exp_data[exp_data.find("[")+1:exp_data.find("]")]
                name = exp_data[:exp_data.find("[")]
                deepnessc = chr(deepness)
                code += f"""for(int {deepnessc} = 0; {tofree}->{name}.len; ++{deepnessc}) {{
{packet_name}_{name} *p_{name} = {deepnessc} * sizeof(*p_{name}) + ({packet_name}_{name} *){tofree}->{name}.data;"""
                code += free_method_content(array_exp, f"p_{name}", deepness+1, packet_name)
                code += f"""}}
FREE({tofree}->{name}.data);
{tofree}->{name}.len = 0;"""
            else:
                code += f"free_{type_map[exp_type][1]}({tofree}->{exp_data});"
    return code

print(free_method_content(exp, "packet", ord("i"), "C2S_play_sus"))