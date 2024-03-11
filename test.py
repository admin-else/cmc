from codegen import type_map


exp = "samogus;vcount;Alol[vsus;sdada;Adigga[[vsudd;sdadauu]]]count"

def split_array_exp(inp):
    first = inp[:inp.find("[")]
    inp = inp[inp.find("[")+1:]
    deepness = 0
    for i, c in enumerate(inp):
        if c == "[":
            deepness += 1
        if c == "]":
            deepness -= 1
            if deepness == -1:
                break
    return first, inp[:i], inp[i+1:]

print(split_array_exp(exp))