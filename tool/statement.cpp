#include "statement.h"

namespace aquarius
{
	std::size_t md5(const std::string& str)
	{
		if (str == "import")
			return 0;
		else if (str == "namespace")
			return 1;
		else if (str == "enum")
			return 2;
		else if (str == "request")
			return 3;
		else if (str == "response")
			return 4;
		else if (str == "int32")
			return 5;
		else if (str == "int64")
			return 6;
		else if (str == "string")
			return 7;
		else if (str == "bool")
			return 8;
		else if (str == "float")
			return 9;
		else if (str == "double")
			return 10;
		else if (str == "uint32")
			return 11;
		else if (str == "uint64")
			return 12;
		else if (str == "bytes")
			return 13;
		else if (str == "message")
			return 14;
		else if (str == "repeated")
			return 15;
		
		return static_cast<std::size_t>( - 1);
	}
}
