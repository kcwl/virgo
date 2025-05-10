from proto_define.base.basic_define import *


def is_proto_value_type(target):
    '''判断是否是协议变量类型'''
    if not isinstance(target, dict) or not dict:
        return False
    
    for val in target.values():
        if val in BASIC_TYPES:
            continue
        if isinstance(val, list) and len(val) == 2 and val[0] == OBJ_T:
            continue
        if isinstance(val, list) and len(val) >= 2 and val[0] == ARRAY_T:
            if len(val) == 2 and val[1] in BASIC_TYPES:
                continue
            if len(val) == 3 and  val[1] == OBJ_T:
                continue    
        return False
    return True

def is_proto_simple_type(target):
    '''简单类型, 如int'''
    if not isinstance(target, str):
        return False
    return target in BASIC_TYPES

def is_proto_obj_type(target):
    '''对象类型, 有结构当以'''
    if not isinstance(target, list) or len(target) != 2:
        return False
    return target[0] == OBJ_T

def is_proto_array_type(target):
    '''数组类型, 如int[]'''
    if not isinstance(target, list) or len(target) < 2:
        return False
    return target[0] == ARRAY_T


def normalize_class_name(s: str, prefix="") -> str:
    """
    把协议的结构体名称转成c++格式
    """
    components = s.split('_')  # 将字符串按下划线分割成组件
    if not components:
        components = [s]  # 如果没有下划线，则返回原字符串
    # 将第一个组件的首字母转换为小写（如果它原本是大写的话），其余组件的首字母转换为大写
    camel_case_str = ''.join(x.capitalize() for x in components)
    camel_case_str = prefix + camel_case_str
    return camel_case_str



def render_struct(struct_name, struct_define, strcut_name_dict, base_indent_str=""):
    '''渲染c++协议数据结构体'''
    camel_case_strcut_name = normalize_class_name(struct_name)
    struct_str = f"{base_indent_str}struct {camel_case_strcut_name}{{\n"
    
    for field_name, field_type in struct_define.items():
        if is_proto_simple_type(field_type):  
            struct_str += f"{base_indent_str}     {field_type} {field_name};\n"
        elif is_proto_obj_type(field_type):
            struct_name = normalize_class_name(strcut_name_dict[id(field_type[1])])
            struct_str += f"{base_indent_str}     {struct_name} {field_name};\n"
        elif is_proto_array_type(field_type):
            if field_type[1] == OBJ_T:
                struct_name = normalize_class_name(strcut_name_dict[id(field_type[2])])
                struct_str += f"{base_indent_str}     std::vector<{struct_name}> {field_name};\n"
            else:
                struct_str += f"{base_indent_str}     std::vector<{field_type[1]}> {field_name};\n"
        else:
            print(f"Unknown Type {field_type}")
            exit(1)

    struct_str += f"{base_indent_str}     void swap({camel_case_strcut_name}& other)\n"
    struct_str += f"{base_indent_str}     {{\n"
    for field_name, field_type in struct_define.items():
        struct_str += f"{base_indent_str}           std::swap({field_name}, other.{field_name});\n"
    struct_str += f"{base_indent_str}     }}\n" 
    struct_str += f"{base_indent_str}}};\n\n"
    return struct_str

def render_struct_reflect(struct_name, struct_define, base_indent_str=""):
    '''渲染c++反射结构体'''
    names = struct_name.split("::")
    camel_case_strcut_name = "::".join([normalize_class_name(name) for name in names])
    struct_str = f"{base_indent_str}template <>\n"   
    struct_str += f"{base_indent_str}struct aquarius::reflect<{camel_case_strcut_name}>\n"
    struct_str += f"{base_indent_str}{{\n"
    struct_str += f"{base_indent_str}     using value_type = {camel_case_strcut_name};\n"
    struct_str += f"{base_indent_str}     constexpr static std::string_view topic()\n"
    struct_str += f"{base_indent_str}     {{\n"
    struct_str += f"{base_indent_str}         return \"{camel_case_strcut_name}\"sv;\n"
    struct_str += f"{base_indent_str}     }}\n"
    struct_str += f"{base_indent_str}     constexpr static std::array<std::string_view, {len(struct_define)}> fields()\n"
    struct_str += f"{base_indent_str}     {{\n"
    struct_str += f"{base_indent_str}         return {{"
    for i, (field_name, _) in enumerate(struct_define.items()):
        if i == 0:
            struct_str += f'"{field_name}"sv'
        else:
            struct_str += f', "{field_name}"sv'
    struct_str += f"}};\n"
    struct_str += f"{base_indent_str}     }}\n"
    struct_str += f"{base_indent_str}}};\n\n"
    return struct_str
