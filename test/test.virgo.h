#pragma once
#include <virgo.hpp>

enum class color
{
    red,
    black,
};
struct person
{
    int age;
    string name;
};
struct hello_protocol
{
private:
    struct hello_req;
    struct hello_resp;
public:
    using request = virgo::tcp::request<hello_req>;
    using response = virgo::tcp::request<hello_req>;
};
