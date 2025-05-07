from ..base.basic_define import *

USER = {
    'sex' : BOOLEAN_T,
    'addr' : UINT32_T,
    'age' : INT_32_T,
    'telephone' : UINT64_T,
    'score' : STRING_T,
    'hp' : FLOAT_T,
    'info' : [REPEATED_T, UINT8_T],
    'name' : STRING_T,
    'orders' : [REPEATED_T, INT_8_T]
}