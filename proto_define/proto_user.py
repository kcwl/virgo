from .struct_desc.user import *
from .base.basic_define import *


PROTO_GID = 1000  # 1000 ~ 2000的协议号为user模块专用



PROTO_GET_USER_INFO = {
    PROTO_ID : PROTO_GID + 1,
    REQUEST: {
        'uid': UINT64_T,
        
    },
    RESPONSE: {
        'user': [OBJ_T, USER],
    }
}


PROTO_GET_SIMPLE_USER_INFO = {
    PROTO_ID : PROTO_GID + 2,
    REQUEST: {
        'uid': UINT64_T,
        
    },
    RESPONSE: {
        'user': [OBJ_T, USER_SIMPLE],
    }
}