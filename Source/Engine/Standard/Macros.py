all_enums = {}

def peaceEnum(name, contents, size=8):
    entries = [c.partition("=")[0].strip() for c in contents.split(",")]
    all_enums[name] = []
    for entry in entries:
        all_enums[name].append(entry)
    return "enum "  + name + " : u" + str(size) + " {" + contents + "};"

def getEnumEntries(enum_name):
    return all_enums[enum_name]

def className(enum_val):
    return enum_val[0] + str.lower(enum_val[1:])
