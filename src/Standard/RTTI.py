# 0 "src/Standard/RTTI.py"
$py(
id_list_filename = makefile_dir + "/ClassIdList.txt"
def uniqueId(classname):
    
    id_list = open(id_list_filename, "r+")
    max_id = 0
    
    for line in id_list:
        name, value_str = line.split()
        value = int(value_str)
        if name == classname:
            return value
        max_id = value if value > max_id else max_id
            
    class_id = max_id + 1
    id_list.write("%s %d\n" % (classname, class_id))
    return class_id
    
cur_class_name = ""
cur_class_id = 0;    

def rttiConstruct(classname):
    global cur_class_name
    global cur_class_id
    cur_class_name = classname
    cur_class_id = uniqueId(classname)
    return "BaseRTTI::type_id = %d" % cur_class_id
  
def registerRttiStruct():
    global cur_class_name
    global cur_class_id
    classname = cur_class_name
    class_id = cur_class_id
    cur_class_name = ""
    cur_class_id = 0;
    return "template <> inline TypeId typeId<%s>() { return %d; }" % (classname, class_id)
)

$extend(rttiConstruct)
$extend(registerRttiStruct)
