#pragma once
#include <virgo.hpp>

struct hello_protocol
{
private:
    struct hello_req;
    struct hello_resp;
public:
    using request = virgo::tcp::request<hello_req>;
    using response = virgo::tcp::request<hello_req>;
};
