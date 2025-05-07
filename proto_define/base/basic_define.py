
# coding: utf-8

'''
    本文件定义协议的基本类型，请勿修改
'''

# 协议基本类型
BOOLEAN_T = 1
UINT8_T = 2
INT_8_T = 3
UINT32_T = 4
INT_32_T = 5
UINT64_T = 6
INT_64_T = 7
FLOAT_T = 8
STRING_T = 9

ARRAY_T = 100
OBJ_T = 101

TYPE_MAP = {
    BOOLEAN_T : 'bool',
    UINT8_T : 'uint8_t',
    INT_8_T : 'int8_t',
    UINT32_T : 'uint32_t',
    INT_32_T : 'int32_t',
    UINT64_T : 'uint64_t',
    INT_64_T : 'int64_t',
    FLOAT_T : 'float',
    STRING_T : 'std::string'
}

PROTO_ID = 'proto_id'
REQUEST = "request"
RESPONSE = "response"


