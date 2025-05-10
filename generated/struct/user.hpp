#include <string>
#include <vector>
#include <message.hpp>
namespace proto_define{

      struct AVATAR{
           uint32_t avatar_id;
           uint8_t avatar_type;
           void swap(AVATAR& other)
           {
                 std::swap(avatar_id, other.avatar_id);
                 std::swap(avatar_type, other.avatar_type);
           }
      };

      template <>
      struct aquarius::reflect<AVATAR>
      {
           using value_type = AVATAR;
           constexpr static std::string_view topic()
           {
               return "AVATAR"sv;
           }
           constexpr static std::array<std::string_view, 2> fields()
           {
               return {"avatar_id"sv, "avatar_type"sv};
           }
      }

      struct ITEM{
           uint32_t item_id;
           uint32_t item_num;
           void swap(ITEM& other)
           {
                 std::swap(item_id, other.item_id);
                 std::swap(item_num, other.item_num);
           }
      };

      template <>
      struct aquarius::reflect<ITEM>
      {
           using value_type = ITEM;
           constexpr static std::string_view topic()
           {
               return "ITEM"sv;
           }
           constexpr static std::array<std::string_view, 2> fields()
           {
               return {"item_id"sv, "item_num"sv};
           }
      }

      struct USER{
           bool sex;
           uint32_t addr;
           int32_t age;
           uint64_t telephone;
           std::string score;
           float hp;
           std::string name;
           AVATAR avatar;
           std::vector<ITEM> items;
           std::vector<int8_t> orders;
           void swap(USER& other)
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
      struct aquarius::reflect<USER>
      {
           using value_type = USER;
           constexpr static std::string_view topic()
           {
               return "USER"sv;
           }
           constexpr static std::array<std::string_view, 10> fields()
           {
               return {"sex"sv, "addr"sv, "age"sv, "telephone"sv, "score"sv, "hp"sv, "name"sv, "avatar"sv, "items"sv, "orders"sv};
           }
      }

      struct USER_SIMPLE{
           std::string score;
           float hp;
           std::string name;
           std::vector<ITEM> items;
           std::vector<int8_t> orders;
           void swap(USER_SIMPLE& other)
           {
                 std::swap(score, other.score);
                 std::swap(hp, other.hp);
                 std::swap(name, other.name);
                 std::swap(items, other.items);
                 std::swap(orders, other.orders);
           }
      };

      template <>
      struct aquarius::reflect<USER_SIMPLE>
      {
           using value_type = USER_SIMPLE;
           constexpr static std::string_view topic()
           {
               return "USER_SIMPLE"sv;
           }
           constexpr static std::array<std::string_view, 5> fields()
           {
               return {"score"sv, "hp"sv, "name"sv, "items"sv, "orders"sv};
           }
      }


}