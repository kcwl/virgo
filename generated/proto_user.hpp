#include <string>
#include <vector>
#include <message.hpp>
#include "struct/user.hpp"


namespace proto_define{

    class PROTO_GET_SIMPLE_USER_INFO
    {
        public:
            PROTO_GET_SIMPLE_USER_INFO(): proto_id(1002){ }

            struct Request{
                 uint64_t uid;
                 void swap(Request& other)
                 {
                       std::swap(uid, other.uid);
                 }
            };

            struct Response{
                 USER_SIMPLE user;
                 void swap(Response& other)
                 {
                       std::swap(user, other.user);
                 }
            };

            uint64_t proto_id;
            Request request;
            Response response;
    };

     template <>
     struct aquarius::reflect<PROTO_GET_SIMPLE_USER_INFO::Request>
     {
          using value_type = PROTO_GET_SIMPLE_USER_INFO::Request;
          constexpr static std::string_view topic()
          {
              return "PROTO_GET_SIMPLE_USER_INFO::Request"sv;
          }
          constexpr static std::array<std::string_view, 1> fields()
          {
              return {"uid"sv};
          }
     }

     template <>
     struct aquarius::reflect<PROTO_GET_SIMPLE_USER_INFO::Response>
     {
          using value_type = PROTO_GET_SIMPLE_USER_INFO::Response;
          constexpr static std::string_view topic()
          {
              return "PROTO_GET_SIMPLE_USER_INFO::Response"sv;
          }
          constexpr static std::array<std::string_view, 1> fields()
          {
              return {"user"sv};
          }
     }

    class PROTO_GET_USER_INFO
    {
        public:
            PROTO_GET_USER_INFO(): proto_id(1001){ }

            struct Request{
                 uint64_t uid;
                 void swap(Request& other)
                 {
                       std::swap(uid, other.uid);
                 }
            };

            struct Response{
                 USER user;
                 void swap(Response& other)
                 {
                       std::swap(user, other.user);
                 }
            };

            uint64_t proto_id;
            Request request;
            Response response;
    };

     template <>
     struct aquarius::reflect<PROTO_GET_USER_INFO::Request>
     {
          using value_type = PROTO_GET_USER_INFO::Request;
          constexpr static std::string_view topic()
          {
              return "PROTO_GET_USER_INFO::Request"sv;
          }
          constexpr static std::array<std::string_view, 1> fields()
          {
              return {"uid"sv};
          }
     }

     template <>
     struct aquarius::reflect<PROTO_GET_USER_INFO::Response>
     {
          using value_type = PROTO_GET_USER_INFO::Response;
          constexpr static std::string_view topic()
          {
              return "PROTO_GET_USER_INFO::Response"sv;
          }
          constexpr static std::array<std::string_view, 1> fields()
          {
              return {"user"sv};
          }
     }

};

