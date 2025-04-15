#define BOOST_TEST_MODULE UnitTest
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

template<>
struct aquarius::reflect<person>
{
	using value_type = person;

	constexpr static std::string_view topic()
	{
		return "person"sv;
	}

	constexpr static std::array<std::string_view, 10> fields()
	{
		return {"sex"sv,"addr"sv,"age"sv,"telephone"sv, "score"sv, "hp"sv, "mana"sv, "info"sv, "name"sv, "orders"sv};
	}
};

class archive
{
public:
	archive()
	{
		buffer_.resize(4096);
	}
public:
	void put(uint8_t value)
	{
		buffer_[wpos_++] = value;
	}

	uint8_t get()
	{
		return buffer_[rpos_++];
	}

	void fill(uint32_t count, uint8_t value)
	{

	}

	auto begin()
	{
		return buffer_.begin();
	}

	auto end()
	{
		return buffer_.end();
	}

	template<typename _Ty>
	void get(_Ty& value)
	{

	}

private:
	int wpos_;
	int rpos_;
	std::vector<uint8_t> buffer_;
};

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
	p1.info = { 1,1,1,1,1,1 };
	p1.name = "John";
	p1.orders = { 1,2,3,4,5 };

	archive ar;

	person p2;

	aquarius::to_binary(ar, p1);
}