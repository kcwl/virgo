from .struct_desc.user import USER
from .base.basic_define import *


PROTO_GID = 100


PROTO_GET_USER_INFO = {
    PROTO_SUB_ID : 1,
    REQUEST: {
        'uid': UINT64_T,
        
    },
    RESPONSE: {
        'user': USER,
    }
}