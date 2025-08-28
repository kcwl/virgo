#include <test.virgo.h>

struct hello::hello_req
{
constexpr static auto id = 1001;
struct header
{
  int id;
};
struct body
{
  int id;
};
};
struct hello::hello_resp
{
constexpr static auto id = 1001;
struct header
{
  int id;
};
struct body
{
  int id;
};
};
