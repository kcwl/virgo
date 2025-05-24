#pragma once
#include <aquarius_protocol/package_processor.hpp>
#include <boost/asio.hpp>
#include <list>
#include <map>

namespace aquarius
{
	namespace tcp
	{
		enum class mvcc
		{
			header = 0x0,
			package = 0x1,
			complete = 0x2,
			normal = package | complete
		};

		struct sequence
		{
			struct block
			{
				std::size_t seq;

				flex_buffer buffer;
			};

			std::size_t total;

			bool has_complete;

			std::list<block> buffers;

			bool check_complete()
			{
				return total == buffers.size();
			}
		};
	} // namespace tcp

	template <>
	struct package_processor<protocol::tcp>
	{
		static constexpr std::size_t package_limit = 4096 - 5;

		auto read(flex_buffer& buffer) -> std::tuple<std::size_t, flex_buffer>
		{
			uint8_t flag{};
			buffer.load(&flag, 1);

			uint32_t req_id{};
			buffer.load((uint8_t*)&req_id, sizeof(uint32_t));

			std::size_t seq = flag >> 2;

			auto& pack = buffers[req_id];

			auto mv = static_cast<tcp::mvcc>(flag & 0x3);

			switch (mv)
			{
			case tcp::mvcc::header:
			case tcp::mvcc::normal:
				{
					pack.total = seq;
					pack.buffers.push_back(tcp::sequence::block{ 0, std::move(buffer) });
				}
				break;
			case tcp::mvcc::complete:
			case tcp::mvcc::package:
				{
					pack.buffers.push_back({ seq, buffer });
				}
				break;
			default:
				break;
			}

			if (mv == tcp::mvcc::normal || mv == tcp::mvcc::complete)
			{
				flex_buffer complete_buffer(pack.buffers.size() * flex_buffer::capacity);

				for (auto iter = pack.buffers.begin(); iter != pack.buffers.end(); iter++)
				{
					complete_buffer.save(iter->buffer.wdata(), iter->buffer.size());
				}

				buffers.erase(req_id);

				return { req_id, complete_buffer };
			}

			return {};
		}

		std::vector<flex_buffer> write(std::size_t proto, flex_buffer& buffer)
		{
			std::vector<flex_buffer> complete_buffers{};

			auto buffer_size = buffer.size();

			uint8_t total = static_cast<uint8_t>((buffer_size + package_limit) / package_limit);

			uint8_t flag = total == 1 ? total << 2 | static_cast<uint8_t>(tcp::mvcc::normal)
									  : total << 2 | static_cast<uint8_t>(tcp::mvcc::header);

			int counter = 0;

			while (true)
			{
				auto cur_size = std::min<std::size_t>(buffer_size, package_limit);

				complete_buffers.push_back({});
				auto& write_buffer = complete_buffers.back();
				write_buffer.save((uint8_t*)&flag, 1);
				write_buffer.save((uint8_t*)&proto, sizeof(uint32_t));
				write_buffer.save((uint8_t*)buffer.wdata(), cur_size);
				buffer.consume(cur_size);
				buffer_size -= cur_size;

				++counter;

				if (buffer_size == 0)
					break;

				buffer_size > package_limit
					? flag = static_cast<uint8_t>(counter << 2) | static_cast<uint8_t>(tcp::mvcc::package)
					: flag = static_cast<uint8_t>(counter << 2) | static_cast<uint8_t>(tcp::mvcc::complete);
			}

			return complete_buffers;
		}

		template <typename Iterator>
		void partical_insert_sort(std::list<tcp::sequence::block>& buffer, Iterator begin, Iterator end,
								  std::size_t pred)
		{
			for (auto iter = begin; iter != end;)
			{
				if (iter->seq != pred)
					continue;

				buffer.insert(begin, std::move(*iter));
				buffer.erase(iter);
			}
		}

		std::map<std::size_t, tcp::sequence> buffers;
	};
} // namespace aquarius