#define BOOST_TEST_MODULE UnitTest
#include <aquarius_protocol.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/test/included/unit_test.hpp>
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

	void swap(person& other)
	{
		std::swap(sex, other.sex);
		std::swap(addr, other.addr);
		std::swap(age, other.age);
		std::swap(telephone, other.telephone);
		std::swap(score, other.score);
		std::swap(hp, other.hp);
		std::swap(mana, other.mana);
		std::swap(info, other.info);
		std::swap(name, other.name);
		std::swap(orders, other.orders);
	}
};

template <>
struct aquarius::reflect<person>
{
	using value_type = person;

	constexpr static std::string_view topic()
	{
		return "person"sv;
	}

	constexpr static std::array<std::string_view, 10> fields()
	{
		return {
			"sex"sv, "addr"sv, "age"sv, "telephone"sv, "score"sv, "hp"sv, "mana"sv, "info"sv, "name"sv, "orders"sv
		};
	}
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

struct tcp
{
	using request_header = aquarius::tcp_request_header;
	using response_header = aquarius::tcp_response_header;

	bool pack(std::vector<char>& buff)
	{
		return true;
	}

	bool unpack(const std::vector<char>& buff)
	{
		return true;
	}
};

BOOST_AUTO_TEST_CASE(tcp_proto)
{
	using person_request = aquarius::basic_request<tcp, person>;
	using person_response = aquarius::basic_response<tcp, person>;
	using person_rpc = aquarius::basic_rpc<person_request, person_response>;

	person_rpc rpc{};
	auto& req = rpc.request();
	req.header()->uuid_ = 1;
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

	std::vector<char> buf = rpc.pack_req();

	person_rpc rpc1{};

	rpc1.unpack_req(buf);

	BOOST_CHECK_EQUAL(*rpc.request().header(), *rpc1.request().header());
	BOOST_CHECK_EQUAL(rpc.request().body(), rpc1.request().body());


	auto& resp = rpc.response();
	resp.header()->uuid_ = 1;
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

	auto resp_buf = rpc.pack_resp();

	rpc1.unpack_resp(resp_buf);

	BOOST_CHECK_EQUAL(*rpc.response().header(), *rpc1.response().header());
	BOOST_CHECK_EQUAL(rpc.response().body(), rpc1.response().body());
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
//BOOST_AUTO_TEST_CASE(ostream_test)
//{
//	using person_request = aquarius::ip::tcp::request<person, 1001>;
//
//	person_request req{};
//
//	boost::asio::streambuf buf;
//
//	std::ostream os(&buf);
//	os << req;
//
//	BOOST_CHECK(!os.fail());
//
//	using person_response = aquarius::ip::tcp::response<person, 1002>;
//
//	person_response resp{};
//
//	os << resp;
//
//	BOOST_CHECK(!os.fail());
//}