#include <lazytool/parse.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_lazy_parse)
{
	std::string message = 
		"message person"
		"{"
		"   int32 a;"
		"   uint32 b;"
		"   int64 c;"
		"   uint64 d;"
		"   string e;"
		"   bytes f;"
		"   repeated int32 g;"
		"	bool h;"
		"   float i;"
		"	double g";
		"}";

	std::string structure = 
		"struct person"
		"{"
		"	int32_t a;"
		"	uint32_t b;"
		"	int64_t c;"
		"   uint64_t d;"
		"   std::string e;"
		"   std::vector<char> f;"
		"   std::vector<int32> g;"
		"	bool h;"
		"   float i;"
		"	double g";

	virgo::parse p{};
}

