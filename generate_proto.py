import proto_define.proto_user as proto_user
from proto_define.base.basic_define import *
import re

import os
import importlib.util
import inspect

def render_struct(struct_name, struct_define, strcut_name_dict, base_indent_str="      "):
    '''渲染c++结构体'''
    struct_str = f"{base_indent_str}struct {struct_name}{{\n"
    for field_name, field_type in struct_define.items():
        if is_proto_simple_type(field_type):  
            struct_str += f"{base_indent_str}     {field_type} {field_name};\n"
        elif is_proto_obj_type(field_type):
            struct_str += f"{base_indent_str}     {strcut_name_dict[id(field_type[1])]} {field_name};\n"
        elif is_proto_array_type(field_type):
            if field_type[1] == OBJ_T:
                struct_str += f"{base_indent_str}     std::vector<{strcut_name_dict[id(field_type[2])]}> {field_name};\n"
            else:
                struct_str += f"{base_indent_str}     std::vector<{field_type[1]}> {field_name};\n"
        else:
            print(f"Unknown Type {field_type}")
            exit(1)
        
    struct_str += f"{base_indent_str}}};\n\n"
    return struct_str



# 定义要搜索的目录路径
proto_dir = './proto_define'  # 请根据实际情况修改路径
struct_dir = './proto_define/struct_desc'
output_path = './generated'

# 遍历目录并过滤出协议文件
proto_files = [f for f in os.listdir(proto_dir) if f.startswith('proto_') and f.endswith('.py')]

# 遍历目录并过滤出结构体文件
struct_files = [f for f in os.listdir(struct_dir) if f.endswith('.py') and f != "__init__.py"]



# 收集所有的结构体定义
struct_dict = {}
strcut_name_dict = {}
struct_module_dict:dict[str,dict] = {}
for struct_file in struct_files:
    file_path = os.path.join(struct_dir, struct_file)
    module_name = struct_file[:-3]  # 去掉文件扩展名.py
    spec = importlib.util.spec_from_file_location(f"proto_define.struct_desc.{module_name}", file_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    
    struct_module_dict[module_name] = {}
    
    # 获取模块中定义的所有成员
    members = inspect.getmembers(module)
    for member in members:
        member_name = member[0]
        member_val = member[1]
        if is_proto_value_type(member[1]):
            if member_name in struct_dict:
                print(f"Redefinition of a Struct {member_name}")
                exit(1)
            struct_dict[member_name] = [module_name, member_val]
            struct_module_dict[module_name][member_name] = member_val
            strcut_name_dict[id(member_val)] = member_name


for module_name, structs in struct_module_dict.items():
    print(f"struct module => {module_name}")
    output_file = os.path.join(output_path, "struct", f"{module_name}.hpp")
    struct_file_str = "#include <string>\n"
    struct_file_str += "#include <vector>\n"
    struct_file_str += "namespace proto_define{\n\n"
    for struct_name, struct_define in structs.items():
         print(f"struct module => {module_name}, st_name = {struct_name}")
         struct_file_str += render_struct(struct_name, struct_define, strcut_name_dict)
    struct_file_str += "\n}"
    with open(output_file, 'w', encoding='utf8') as f:
        f.write(struct_file_str)
            

# 收集协议定义
for proto_file in proto_files:
    file_path = os.path.join(proto_dir, proto_file)
    module_name = proto_file[:-3]  # 去掉文件扩展名.py
    spec = importlib.util.spec_from_file_location(f"proto_define.{module_name}", file_path)
    module = importlib.util.module_from_spec(spec)
    print(module_name, file_path)
    spec.loader.exec_module(module)
    
    # 获取模块中定义的所有成员
    members = inspect.getmembers(module)

    # 过滤出协议定义
    proto_gid = module.PROTO_GID
    proto_defines = []
    strcut_name_dict = {} 
    
    for name, value in members:
        if isinstance(value, dict) and all(key in value for key in (PROTO_ID, REQUEST, RESPONSE)):
            proto_defines.append((name, value))
        elif is_proto_value_type(value):
            strcut_name_dict[id(value)] = name
    
    # 开始执行协议生成逻辑
    print(f'proto_gid = {proto_gid}')
    
    depend_structs = set()
    depend_modules = set()
    for proto_name, proto_define in proto_defines:
        proto_id = proto_define[PROTO_ID]
        
        # 检查协议id是否合法
        if proto_id < proto_gid or proto_id > proto_gid + 1000:
            raise ValueError(f"Invalid proto_id {proto_id} for {proto_name}")
        
        req_st = proto_define[REQUEST]
        resp_st = proto_define[RESPONSE]
        
        
        for st in [req_st, resp_st]:       
            for field_name, field_type in st.items():
                struct_desc_ref = None
                if is_proto_obj_type(field_type):
                    struct_desc_ref = field_type[1]
                elif is_proto_array_type(field_type) and field_type[1] == OBJ_T:
                    struct_desc_ref = field_type[2]
                if struct_desc_ref:
                    struct_name = strcut_name_dict[id(struct_desc_ref)]
                    depend_structs.add(struct_name)
            
    proto_file_str = ""
    for struct_name in depend_structs:
        module_name, member_val = struct_dict[struct_name]
        depend_modules.add(module_name)
        
    proto_file_str = "#include <string>\n"
    proto_file_str += "#include <vector>\n"
        
    for module_name in depend_modules:
        proto_file_str += f"#include \"struct/{module_name}.hpp\"\n"
    proto_file_str += f"\n\n"
    proto_file_str += "namespace proto_define{\n\n"
    for proto_name, proto_define in proto_defines:
        proto_id = proto_define[PROTO_ID]
        proto_file_str += f"    class {proto_name}\n    {{\n        public:\n"
        proto_file_str += f"            {proto_name}(): proto_id({proto_id}){{ }}\n\n"
        
        req_st = proto_define[REQUEST]
        resp_st = proto_define[RESPONSE]
        proto_file_str += render_struct("Request", req_st, strcut_name_dict, "            ")
        proto_file_str += render_struct("Request", resp_st, strcut_name_dict, "            ")
        
        proto_file_str += f"        private:\n"
        proto_file_str += f"            uint64_t proto_id;\n"
        proto_file_str += f"    }};\n\n"
    proto_file_str += f"}};\n\n"    
    proto_file_path = os.path.join(output_path, f"proto_{module_name}.hpp")
    with open(proto_file_path, 'w', encoding='utf8') as f:
        f.write(proto_file_str)
            
            
        # for proto_st_type in  [REQUEST, RESPONSE]:
        #     proto_st:dict = proto_define[proto_st_type]
        #     for property_name, property_desc in proto_st.items():
        #         if not is_proto_value_type(property_desc):
        #             print(f"Property '{property_name}' has invalid type '{property_desc}'.")
        #             exit(1)
                
            

            
    
