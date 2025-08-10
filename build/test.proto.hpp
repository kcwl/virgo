#pragma once
#include <virgo.hpp>


enum class color
{
	red,
	yellow,
	black
};

struct test_structure_req
{
	int32_t a;
	uint32_t b;
	int64_t c;
	uint64_t d;
	bool e;
	std::string f;
	std::vector<char> g;
	float h;
	double i;
};

struct test_structure_resp
{
	int32_t a;
	uint32_t b;
	int64_t c;
	uint64_t d;
	bool e;
	std::string f;
	std::vector<char> g;
	float h;
	double i;
};
struct rpc_test_structure
{
	constexpr static auto id = "1001"sv;
	using request = virgo::tcp::request<test_structure>;
	using response = virgo::tcp::response<test_structure>;
};


struct rpc_test_structure
{
	constexpr static auto id = "/login"sv;
	using request = virgo::http::request<test_structure>;
	using response = virgo::http::response<test_structure>;
};


