
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
