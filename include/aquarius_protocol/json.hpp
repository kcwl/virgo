#pragma once
#include <boost/json.hpp>
#include <string>

namespace aquarius
{
	namespace serialize
	{
		template <typename T>
		std::string to_json(const T& value)
		{
			auto value = boost::json::value_from(value);

			return boost::json::serialize(value);
		}

		template <typename T>
		T from_json(const std::string& json)
		{
			auto value = boost::json::parse(json);

			return boost::json::value_to<T>(value);
		}
	} // namespace serialize
} // namespace aquarius