#pragma once
#include <boost/asio.hpp>
#include <aquarius_protocol/package_processor.hpp>
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

		template<typename FlexBuffer>
		struct sequence
		{
			struct block
			{
				std::size_t seq;

				FlexBuffer buffer;
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
	template<typename FlexBuffer>
	struct package_processor<FlexBuffer, boost::asio::ip::tcp>
	{
		static constexpr std::size_t package_limit = 4096;

		auto read(FlexBuffer& buffer) -> std::tuple<std::size_t, FlexBuffer>
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
				int counter = 0;

				FlexBuffer complete_buffer;

				for (auto iter = pack.buffers.begin(); iter != pack.buffers.end();)
				{
					if (iter->seq == counter++)
					{
						complete_buffer.save(iter->buffer.wdata(), iter->buffer.size());

						iter++;
					}
					else
					{
						partical_insert_sort(pack.buffers, iter, pack.buffers.end(), counter);
					}
				}

				buffers.erase(req_id);

				return { req_id, complete_buffer };
			}
		}

		
		std::vector<FlexBuffer> write(std::size_t proto, FlexBuffer buffer)
		{
			std::vector<FlexBuffer> complete_buffers{};

			auto buffer_size = buffer.size();

			uint8_t total = static_cast<uint8_t>(buffer_size / package_limit) + 1;

			uint8_t flag = (total << 2) | static_cast<uint8_t>(tcp::mvcc::header);

			int counter = 0;

			while (buffer_size != 0)
			{
				auto cur_size = std::min<std::size_t>(buffer_size, package_limit);

				if (counter != 0) [[unlikely]]
				{
					cur_size < buffer_size
						? flag = static_cast<uint8_t>(counter << 2) | static_cast<uint8_t>(tcp::mvcc::package)
						: flag = static_cast<uint8_t>(counter << 2) | static_cast<uint8_t>(tcp::mvcc::complete);
				}

				complete_buffers.push_back({});
				auto& write_buffer = complete_buffers.back();
				write_buffer.save((uint8_t*)&flag, 1);
				write_buffer.save((uint8_t*)&proto, sizeof(uint32_t));
				write_buffer.save((uint8_t*)buffer.wdata(), cur_size);
				buffer_size -= cur_size;

				++counter;
			}

			return complete_buffers;
		}

		template <typename Iterator, typename FlexBuffer>
		void partical_insert_sort(std::list<typename tcp::sequence<FlexBuffer>::block>& buffer, Iterator begin, Iterator end,
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

		std::map<std::size_t, typename tcp::sequence<FlexBuffer>> buffers;
	};
} // namespace aquarius