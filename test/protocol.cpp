#define BOOST_TEST_MODULE UnitTest
#include <boost/asio/streambuf.hpp>
#include <boost/test/included/unit_test.hpp>
#include <message.hpp>

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

BOOST_AUTO_TEST_CASE(binary)
{
	person p1{};
	p1.sex = true;
	p1.addr = 2;
	p1.age = 15;
	p1.telephone = 15230214856;
	p1.score = 100;
	p1.hp = 200;
	p1.mana = 300;
	p1.info = { 1, 1, 1, 1, 1, 1 };
	p1.name = "John";
	p1.orders = { 1, 2, 3, 4, 5 };

	boost::asio::streambuf ar{};

	aquarius::binary::to(ar, p1);

	person p2 = aquarius::binary::from<person>(ar);

	BOOST_CHECK_EQUAL(p1, p2);
}

BOOST_AUTO_TEST_CASE(request)
{
	using person_request = aquarius::basic_request<person, 1001>;

	person_request req{};
	req.header()->total_package = 1;
	req.header()->pack_seq = 1;
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

	aquarius::archive ar{};

	req.disperse(ar, 4096);

	person_request req1{};

	req1.collect(ar);

	req1.complete();

	BOOST_CHECK_EQUAL(req, req1);
}