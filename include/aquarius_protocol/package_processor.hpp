#pragma once

namespace aquarius
{
	enum class protocol
	{
		tcp,
		udp,
		http
	};

	template <protocol Protocol>
	struct package_processor;
} // namespace aquarius