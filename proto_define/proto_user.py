from .struct_desc.user import *
from .base.basic_define import *


PROTO_GID = 1000  # 1000 ~ 2000的协议号为user模块专用


PROTO_GET_USER_INFO = {
    PROTO_ID : PROTO_GID + 1,
    REQUEST: {
        'uid': UINT64_T,
        
    },
    RESPONSE: {
        'user': ST_USER,
    }
}