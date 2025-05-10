#include <string>
#include <vector>
#include "struct/user.hpp"


    class ProtoGetUserInfo
    {
        public:
            ProtoGetUserInfo(): proto_id(1001){ }

            struct Request{
                uint64_t uid;
            };

            struct Request{
                USER user;
            };

    };

