#pragma once
#include <ostream>

namespace aquarius
{
	enum class http_version
	{
		http1_0,
		http1_1,
		http2,
		http3
	};

	inline std::ostream& operator<<(std::ostream& os, http_version v)
	{
		os << static_cast<int>(v);

		return os;
	}
} // namespace aquarius