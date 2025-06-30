#define BOOST_TEST_MODULE UnitTest
#include <aquarius_protocol.hpp>
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
	using request = aquarius::tcp_request<person>;
	using response = aquarius::tcp_response<person>;
};

BOOST_AUTO_TEST_CASE(tcp_proto)
{
	rpc_person::request req{};
	req.header()->uuid(1);
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
	req.pack(buf);

	rpc_person::request req1{};

	req1.unpack(buf);

	BOOST_CHECK_EQUAL(req, req1);


	rpc_person::response resp{};
	resp.header()->uuid(1);
	resp.header()->result(1);
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
	resp.pack(resp_buf);

	rpc_person::response resp1{};
	resp1.unpack(resp_buf);

	BOOST_CHECK_EQUAL(resp, resp1);
}

//BOOST_AUTO_TEST_CASE(multi_tcp_processor)
//{
//	using person_request = aquarius::ip::tcp::request<person, 1001>;
//
//	person_request req{};
//	req.header()->crc32_ = 1;
//	req.header()->timestamp_ = 1;
//	req.body().sex = true;
//	req.body().addr = 2;
//	req.body().age = 15;
//	req.body().telephone = 15230214856;
//	req.body().score = 100;
//	req.body().hp = 200;
//	req.body().mana = 300;
//	req.body().info = { 1, 1, 1, 1, 1, 1 };
//	req.body().name = "John";
//	req.body().orders.resize(4096);
//	std::fill(req.body().orders.begin(), req.body().orders.end(), 4);
//
//	aquarius::flex_buffer ar{};
//	BOOST_CHECK(req.to_binary(ar));
//
//	aquarius::package_processor<aquarius::protocol::tcp> processer;
//
//	auto req_buffer = processer.write(1001, ar);
//
//	for (auto& r : req_buffer)
//	{
//		auto [proto, buffer] = processer.read(r);
//
//		if (buffer.size() == 0)
//			continue;
//
//		BOOST_CHECK_EQUAL(proto, 1001);
//
//		person_request req1{};
//
//		BOOST_CHECK(req1.from_binary(buffer));
//
//		BOOST_CHECK_EQUAL(req1, req);
//
//		break;
//	}
//
//	using person_response = aquarius::ip::tcp::response<person, 1002>;
//
//	person_response resp{};
//	resp.header()->crc32_ = 1;
//	resp.header()->timestamp_ = 1;
//	resp.body().sex = true;
//	resp.body().addr = 2;
//	resp.body().age = 15;
//	resp.body().telephone = 15230214856;
//	resp.body().score = 100;
//	resp.body().hp = 200;
//	resp.body().mana = 300;
//	resp.body().info = { 1, 1, 1, 1, 1, 1 };
//	resp.body().name = "John";
//	resp.body().orders.resize(4096);
//	std::fill(resp.body().orders.begin(), resp.body().orders.end(), 4);
//
//	aquarius::flex_buffer write_buffer{};
//	BOOST_CHECK(resp.to_binary(write_buffer));
//
//	auto resp_buffer = processer.write(1002, write_buffer);
//
//	for (auto& r : resp_buffer)
//	{
//		auto [resp_proto, buffer] = processer.read(r);
//
//		if (buffer.size() == 0)
//			continue;
//
//		BOOST_CHECK_EQUAL(resp_proto, 1002);
//
//		person_response resp1{};
//		BOOST_CHECK(resp1.from_binary(buffer));
//
//		BOOST_CHECK_EQUAL(resp, resp1);
//	}
//}
//
//BOOST_AUTO_TEST_CASE(null_buffer)
//{
//	using person_request = aquarius::ip::tcp::request<person, 1001>;
//
//	person_request req{};
//	req.header()->crc32_ = 1;
//	req.header()->timestamp_ = 1;
//	req.body().sex = true;
//	req.body().addr = 2;
//	req.body().age = 15;
//	req.body().telephone = 15230214856;
//	req.body().score = 100;
//	req.body().hp = 200;
//	req.body().mana = 300;
//	req.body().info = { 1, 1, 1, 1, 1, 1 };
//	req.body().name = "John";
//	req.body().orders.resize(4096);
//	std::fill(req.body().orders.begin(), req.body().orders.end(), 4);
//
//	aquarius::flex_buffer ar(0);
//
//	person_request req1{};
//	BOOST_CHECK(!req.from_binary(ar));
//}
//

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
	aquarius::http_request<std::string> req{};
	// header
	{
		req.header()->method(aquarius::http_method::get);

		BOOST_CHECK_EQUAL(req.header()->method(), aquarius::http_method::get);

		req.header()->version(aquarius::http_version::http1_1);

		BOOST_CHECK_EQUAL(req.header()->version(), aquarius::http_version::http1_1);

		req.header()->path("/api/v1/create");

		BOOST_CHECK_EQUAL(req.header()->path(), "/api/v1/create");

		req.header()->querys().push_back({ "has_login","false" });

		auto& first_query = req.header()->querys().at(0);

		BOOST_CHECK_EQUAL(first_query.first, "has_login");

		BOOST_CHECK_EQUAL(first_query.second, "false");

		test_streambuf buf;

		std::ostream os(&buf);

		os << req;

		BOOST_CHECK(os.fail());
	}

	// request header
	{
		req.header()->set_field(aquarius::http_field_type::connection, "keep-alive");

		BOOST_CHECK(req.header()->keep_alive());

		req.header()->set_chunked(true);

		BOOST_CHECK(req.header()->chunked());

		req.header()->content_length(16);

		BOOST_CHECK(req.header()->has_content_length());

		BOOST_CHECK_EQUAL(*req.header()->content_length(), 16);

		req.header()->keep_alive(false);

		BOOST_CHECK(!req.header()->keep_alive());
	}
}