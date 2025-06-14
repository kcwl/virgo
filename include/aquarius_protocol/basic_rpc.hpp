#pragma once
#include <vector>

namespace aquarius
{
	template <typename Request, typename Response>
	class basic_rpc
	{
	public:
		using request_type = Request;

		using response_type = Response;

	public:
		void unpack_req(std::vector<char>& buff)
		{
			request_.unpack(buff);
		}

		void unpack_resp(std::vector<char>& buff)
		{
			response_.unpack(buff);
		}

		std::vector<char> pack_req()
		{
			std::vector<char> buff{};

			request_.pack(buff);

			return buff;
		}

		std::vector<char> pack_resp()
		{
			std::vector<char> buff{};

			response_.pack(buff);

			return buff;
		}

		request_type& request()
		{
			return request_;
		}

		response_type& response()
		{
			return response_;
		}

	private:
		request_type request_;

		response_type response_;
	};
} // namespace aquarius