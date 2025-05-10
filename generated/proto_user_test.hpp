
#include "struct/user.hpp"



namespace proto_define
{

    class ProtoGetUserInfo
    {
        public:
            ProtoGetUserInfo(): proto_id(1001)
            {

            }

            class Request{
                uint64_t uid;

            };

            class Response{
                User user;
            };

        private:
            uint64_t proto_id;
    }
}