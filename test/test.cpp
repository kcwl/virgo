#define BOOST_TEST_MODULE UnitTest
#include <virgo.hpp>
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <iostream>


struct person
{
	bool sex;
	uint32_t addr;
	int32_t age;
	uint64_t telephone;
	int64_t score;
	float hp;
	double mana;
	std::vector<uint8_t> info;
	std::string name;
	std::vector<int> orders;
};

bool operator==(const person& lhs, const person& rhs)
{
	return lhs.sex == rhs.sex && lhs.addr == rhs.addr && lhs.age == rhs.age && lhs.telephone == rhs.telephone &&
		   lhs.score == rhs.score && lhs.hp == rhs.hp && lhs.mana && rhs.mana && lhs.info == rhs.info &&
		   lhs.name == rhs.name && lhs.orders == rhs.orders;
}

std::ostream& operator<<(std::ostream& os, const person& p)
{
	os << p.sex << "," << p.addr << "," << p.age << "," << p.telephone << "," << p.score << "," << p.hp << "," << p.mana
	   << ", [";

	for (auto& v : p.info)
	{
		os << v << ",";
	}

	os.seekp(-1, std::ios::cur);

	os << "]," << p.name << ", [";

	for (auto& v : p.info)
	{
		os << v << ",";
	}

	os.seekp(-1, std::ios::cur);

	os << "]\n";

	return os;
}

struct rpc_person
{
	using request = virgo::tcp::request<person>;
	using response = virgo::tcp::response<person>;
};

BOOST_AUTO_TEST_CASE(tcp_proto)
{
	rpc_person::request req{};
	req.header().uuid(1);
	req.body().sex = true;
	req.body().addr = 2;
	req.body().age = 15;
	req.body().telephone = 15230214856;
	req.body().score = 100;
	req.body().hp = 200;
	req.body().mana = 300;
	req.body().info = { 1, 1, 1, 1, 1, 1 };
	req.body().name = "John";
	req.body().orders = { 1, 2, 3, 4, 5 };

	std::vector<char> buf;
	req.commit(buf);

	rpc_person::request req1{};

	req1.consume(buf);

	BOOST_CHECK_EQUAL(req, req1);


	rpc_person::response resp{};
	resp.header().uuid(1);
	resp.header().result(1);
	resp.body().sex = true;
	resp.body().addr = 2;
	resp.body().age = 15;
	resp.body().telephone = 15230214856;
	resp.body().score = 100;
	resp.body().hp = 200;
	resp.body().mana = 300;
	resp.body().info = { 1, 1, 1, 1, 1, 1 };
	resp.body().name = "John";
	resp.body().orders = { 1, 2, 3, 4, 5 };

	std::vector<char> resp_buf{};
	resp.commit(resp_buf);

	rpc_person::response resp1{};
	resp1.consume(resp_buf);

	BOOST_CHECK_EQUAL(resp, resp1);
}

class test_streambuf : public std::streambuf {};

BOOST_AUTO_TEST_CASE(for_ostream_test)
{
	test_streambuf buf;

	std::ostream os(&buf);

	rpc_person::request req{};

	os << req;

	BOOST_CHECK(os.fail());

	rpc_person::response resp{};

	os << resp;

	BOOST_CHECK(os.fail());
}

BOOST_AUTO_TEST_CASE(http_req)
{
	virgo::http::request<std::string> req{};
	// header
	{
		req.base_header().method(virgo::http::method::get);

		BOOST_CHECK_EQUAL(req.base_header().method(), virgo::http::method::get);

		req.base_header().version(virgo::http::version::http1_1);

		BOOST_CHECK_EQUAL(req.base_header().version(), virgo::http::version::http1_1);

		req.base_header().path("/api/v1/create");

		BOOST_CHECK_EQUAL(req.base_header().path(), "/api/v1/create");

		req.base_header().querys().push_back({ "has_login","false" });

		auto& first_query = req.base_header().querys().at(0);

		BOOST_CHECK_EQUAL(first_query.first, "has_login");

		BOOST_CHECK_EQUAL(first_query.second, "false");

		test_streambuf buf;

		std::ostream os(&buf);

		os << req;

		BOOST_CHECK(os.fail());
	}

	// request header
	{
		req.header().keep_alive(true);

		BOOST_CHECK(req.header().keep_alive());

		req.header().content_length(16);

		BOOST_CHECK(req.header().content_length());

		BOOST_CHECK_EQUAL(*req.header().content_length(), 16);

		req.header().keep_alive(false);

		BOOST_CHECK(!req.header().keep_alive());
	}
}