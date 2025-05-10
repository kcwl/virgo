#pragma once
#include <string>
#include <vector>
#include <message.hpp>

struct Avatar{
     uint32_t avatar_id;
     uint64_t avatar_type;
     void swap(Avatar& other)
     {
           std::swap(avatar_id, other.avatar_id);
           std::swap(avatar_type, other.avatar_type);
     }
};

template <>
struct aquarius::reflect<Avatar>
{
     using value_type = Avatar;
     constexpr static std::string_view topic()
     {
         return "Avatar"sv;
     }
     constexpr static std::array<std::string_view, 2> fields()
     {
         return {"avatar_id"sv, "avatar_type"sv};
     }
};

struct Item{
     uint32_t item_id;
     uint32_t item_num;
     void swap(Item& other)
     {
           std::swap(item_id, other.item_id);
           std::swap(item_num, other.item_num);
     }
};

template <>
struct aquarius::reflect<Item>
{
     using value_type = Item;
     constexpr static std::string_view topic()
     {
         return "Item"sv;
     }
     constexpr static std::array<std::string_view, 2> fields()
     {
         return {"item_id"sv, "item_num"sv};
     }
};

struct User{
     bool sex;
     uint32_t addr;
     int32_t age;
     uint64_t telephone;
     std::string score;
     float hp;
     std::string name;
     Avatar avatar;
     std::vector<Item> items;
     std::vector<int8_t> orders;
     void swap(User& other)
     {
           std::swap(sex, other.sex);
           std::swap(addr, other.addr);
           std::swap(age, other.age);
           std::swap(telephone, other.telephone);
           std::swap(score, other.score);
           std::swap(hp, other.hp);
           std::swap(name, other.name);
           std::swap(avatar, other.avatar);
           std::swap(items, other.items);
           std::swap(orders, other.orders);
     }
};

template <>
struct aquarius::reflect<User>
{
     using value_type = User;
     constexpr static std::string_view topic()
     {
         return "User"sv;
     }
     constexpr static std::array<std::string_view, 10> fields()
     {
         return {"sex"sv, "addr"sv, "age"sv, "telephone"sv, "score"sv, "hp"sv, "name"sv, "avatar"sv, "items"sv, "orders"sv};
     }
};

struct UserSimple{
     std::string score;
     float hp;
     std::string name;
     std::vector<Item> items;
     std::vector<int8_t> orders;
     void swap(UserSimple& other)
     {
           std::swap(score, other.score);
           std::swap(hp, other.hp);
           std::swap(name, other.name);
           std::swap(items, other.items);
           std::swap(orders, other.orders);
     }
};

template <>
struct aquarius::reflect<UserSimple>
{
     using value_type = UserSimple;
     constexpr static std::string_view topic()
     {
         return "UserSimple"sv;
     }
     constexpr static std::array<std::string_view, 5> fields()
     {
         return {"score"sv, "hp"sv, "name"sv, "items"sv, "orders"sv};
     }
};

