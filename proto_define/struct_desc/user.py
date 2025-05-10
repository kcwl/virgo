from ..base.basic_define import *


AVATAR = {
    'avatar_id' : UINT32_T,
    'avatar_type' : UINT8_T
}

ITEM = {
    'item_id' : UINT32_T,
    'item_num' : UINT32_T
}

USER = {
    'sex' : BOOLEAN_T,
    'addr' : UINT32_T,
    'age' : INT_32_T,
    'telephone' : UINT64_T,
    'score' : STRING_T,
    'hp' : FLOAT_T,
    'name' : STRING_T,
    'avatar' : [OBJ_T, AVATAR],
    'items' : [ARRAY_T, OBJ_T, ITEM],
    'orders' : [ARRAY_T, INT_8_T]
}



USER_SIMPLE = {
    'score' : STRING_T,
    'hp' : FLOAT_T,
    'name' : STRING_T,
    'items' : [ARRAY_T, OBJ_T, ITEM],
    'orders' : [ARRAY_T, INT_8_T]
}

