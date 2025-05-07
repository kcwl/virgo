from ..base.basic_define import *

ST_AVATAR = {
    'avatar_id' : UINT32_T,
    'avatar_type' : UINT8_T
}

ST_ITEM = {
    'item_id' : UINT32_T,
    'item_num' : UINT32_T
}

ST_USER = {
    'sex' : BOOLEAN_T,
    'addr' : UINT32_T,
    'age' : INT_32_T,
    'telephone' : UINT64_T,
    'score' : STRING_T,
    'hp' : FLOAT_T,
    'name' : STRING_T,
    'avatar' : [OBJ_T, ST_AVATAR],
    'items' : [ARRAY_T, OBJ_T, ST_ITEM],
    'orders' : [ARRAY_T, INT_8_T]
}