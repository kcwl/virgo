#define BOOST_TEST_MODULE UnitTest
#include <boost/asio/streambuf.hpp>
#include <boost/test/included/unit_test.hpp>
#include <serialize.hpp>

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

	aquarius::binary<boost::asio::streambuf>::to(ar, p1);

	person p2 = aquarius::binary<boost::asio::streambuf>::template from<person>(ar);

	BOOST_CHECK_EQUAL(p1, p2);
}