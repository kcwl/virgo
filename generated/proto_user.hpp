#pragma once
#include <string>
#include <vector>
#include <message.hpp>
#include "struct/user.hpp"


class ProtoGetSimpleUserInfo
{
   public:
        ProtoGetSimpleUserInfo(): proto_id(1002){ }

        struct Request{
             uint64_t uid;
             void swap(Request& other)
             {
                   std::swap(uid, other.uid);
             }
        };

        struct Response{
             UserSimple user;
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
struct aquarius::reflect<ProtoGetSimpleUserInfo::Request>
{
     using value_type = ProtoGetSimpleUserInfo::Request;
     constexpr static std::string_view topic()
     {
         return "ProtoGetSimpleUserInfo::Request"sv;
     }
     constexpr static std::array<std::string_view, 1> fields()
     {
         return {"uid"sv};
     }
};

template <>
struct aquarius::reflect<ProtoGetSimpleUserInfo::Response>
{
     using value_type = ProtoGetSimpleUserInfo::Response;
     constexpr static std::string_view topic()
     {
         return "ProtoGetSimpleUserInfo::Response"sv;
     }
     constexpr static std::array<std::string_view, 1> fields()
     {
         return {"user"sv};
     }
};

class ProtoGetUserInfo
{
   public:
        ProtoGetUserInfo(): proto_id(1001){ }

        struct Request{
             uint64_t uid;
             void swap(Request& other)
             {
                   std::swap(uid, other.uid);
             }
        };

        struct Response{
             User user;
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
struct aquarius::reflect<ProtoGetUserInfo::Request>
{
     using value_type = ProtoGetUserInfo::Request;
     constexpr static std::string_view topic()
     {
         return "ProtoGetUserInfo::Request"sv;
     }
     constexpr static std::array<std::string_view, 1> fields()
     {
         return {"uid"sv};
     }
};

template <>
struct aquarius::reflect<ProtoGetUserInfo::Response>
{
     using value_type = ProtoGetUserInfo::Response;
     constexpr static std::string_view topic()
     {
         return "ProtoGetUserInfo::Response"sv;
     }
     constexpr static std::array<std::string_view, 1> fields()
     {
         return {"user"sv};
     }
};

