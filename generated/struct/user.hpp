#include <string>
#include <vector>
namespace proto_define{

      struct AVATAR{
           uint32_t avatar_id;
           uint8_t avatar_type;
      };

      struct ITEM{
           uint32_t item_id;
           uint32_t item_num;
      };

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
      };

      struct USER_SIMPLE{
           std::string score;
           float hp;
           std::string name;
           std::vector<ITEM> items;
           std::vector<int8_t> orders;
      };


}