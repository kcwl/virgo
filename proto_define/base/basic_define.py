
# coding: utf-8

'''
    本文件定义协议的基本类型，请勿修改
'''

# 协议基本类型
BOOLEAN_T = "bool"
UINT8_T = 'uint8_t'
INT_8_T = 'int8_t'
UINT32_T = 'uint32_t'
INT_32_T = 'int32_t'
UINT64_T = 'uint64_t'
INT_64_T = 'int64_t'
FLOAT_T = 'float'
STRING_T = 'std::string'

ARRAY_T = "repeated_t"
OBJ_T = "reflactable"

BASIC_TYPES = [
    BOOLEAN_T,
    UINT8_T,
    INT_8_T,
    UINT32_T,
    INT_32_T,
    UINT64_T,
    INT_64_T,
    FLOAT_T,
    STRING_T
]

PROTO_ID = 'proto_id'
REQUEST = "request"
RESPONSE = "response"

def is_proto_value_type(target):
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
    if not isinstance(target, str):
        return False
    return target in BASIC_TYPES

def is_proto_obj_type(target):
    if not isinstance(target, list) or len(target) != 2:
        return False
    return target[0] == OBJ_T

def is_proto_array_type(target):
    if not isinstance(target, list) or len(target) < 2:
        return False
    return target[0] == ARRAY_T