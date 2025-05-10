#include <string>
#include <vector>
#include "struct/user.hpp"


namespace proto_define{

    class PROTO_GET_SIMPLE_USER_INFO
    {
        public:
            PROTO_GET_SIMPLE_USER_INFO(): proto_id(1002){ }

            struct Request{
                 uint64_t uid;
            };

            struct Request{
                 USER_SIMPLE user;
            };

        private:
            uint64_t proto_id;
    };

    class PROTO_GET_USER_INFO
    {
        public:
            PROTO_GET_USER_INFO(): proto_id(1001){ }

            struct Request{
                 uint64_t uid;
            };

            struct Request{
                 USER user;
            };

        private:
            uint64_t proto_id;
    };

};

