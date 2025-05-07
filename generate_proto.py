import proto_define.proto_user as proto_user
from proto_define.base.basic_define import *


import os
import importlib.util
import inspect

# 定义要搜索的目录路径
directory_path = './proto_define'  # 请根据实际情况修改路径
output_path = './generated'

# 遍历目录并过滤文件
proto_files = [f for f in os.listdir(directory_path) if f.startswith('proto_') and f.endswith('.py')]

# 动态加载模块
for proto_file in proto_files:
    file_path = os.path.join(directory_path, proto_file)
    module_name = proto_file[:-3]  # 去掉文件扩展名.py
    spec = importlib.util.spec_from_file_location(f"proto_define.{module_name}", file_path)
    module = importlib.util.module_from_spec(spec)
    print(module_name, file_path)
    spec.loader.exec_module(module)
    
    # 获取模块中定义的所有成员
    members = inspect.getmembers(module)

    # 过滤出协议定义
    proto_gid = module.PROTO_GID
    proto_defines = []
    for name, value in members:
        if isinstance(value, dict) and all(key in value for key in (PROTO_ID, REQUEST, RESPONSE)):
            proto_defines.append((name, value))
    
    # 开始执行协议生成逻辑
    print(f'proto_gid = {proto_gid}')
    for proto_name, proto_define in proto_defines:
        proto_id = proto_define[PROTO_ID]
        
        # 检查协议id是否合法
        if proto_id < proto_gid or proto_id > proto_gid + 1000:
            raise ValueError(f"Invalid proto_id {proto_id} for {proto_name}")

        