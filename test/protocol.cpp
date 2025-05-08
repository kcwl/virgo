#define BOOST_TEST_MODULE UnitTest
#include <boost/asio/streambuf.hpp>
#include <boost/test/included/unit_test.hpp>
#include <aquarius_protocol.hpp>

namespace detail
{
	template <typename _Elem, typename _Traits, typename _Alloc = std::allocator<_Elem>>
	class flex_buffer
	{
		using elem_type = _Elem;

		using allocator_type = _Alloc;

		using traits_type = _Traits;

		using this_type = flex_buffer<elem_type, traits_type, allocator_type>;

		constexpr static std::size_t water_line = 32;

	public:
		constexpr static std::size_t capacity = 512;

	public:
		using iterator = typename std::vector<elem_type, allocator_type>::iterator;
		using const_iterator = typename std::vector<elem_type, allocator_type>::const_iterator;
		using value_type = typename std::vector<elem_type, allocator_type>::value_type;
		using size_type = typename std::vector<elem_type, allocator_type>::size_type;
		using reference = typename std::vector<elem_type, allocator_type>::reference;
		using const_reference = typename std::vector<elem_type, allocator_type>::const_reference;
		using pointer = typename std::vector<elem_type, allocator_type>::pointer;
		using const_pointer = typename std::vector<elem_type, allocator_type>::const_pointer;

		using seekdir = std::ios::seekdir;
		using openmode = std::ios::openmode;

		using pos_type = typename traits_type::pos_type;
		using off_type = typename traits_type::off_type;

	public:
		flex_buffer(const std::size_t capa = capacity)
			: buffer_(capa, 0)
			, pptr_(0)
			, gptr_(0)
			, pcount_(capa)
			, start_(0)
			, has_success_(true)
		{
		}

		flex_buffer(flex_buffer&& other)
			: buffer_(std::move(other.buffer_))
			, pptr_(std::exchange(other.pptr_, 0))
			, gptr_(std::exchange(other.gptr_, 0))
			, pcount_(std::exchange(other.pcount_, 0))
			, start_(std::exchange(other.start_, 0))
			, has_success_(std::exchange(other.has_success_, true))
		{
		}

		flex_buffer(const flex_buffer&) = default;
		flex_buffer& operator=(const flex_buffer&) = default;

		flex_buffer& operator=(flex_buffer&& other) noexcept
		{
			if (this != std::addressof(other))
			{
				buffer_ = std::move(other.buffer_);
				pptr_ = std::exchange(other.pptr_, 0);
				gptr_ = std::exchange(other.gptr_, 0);
				pcount_ = std::exchange(other.pcount_, 0);
				start_ = std::exchange(other.start_, 0);
				has_success_ = std::exchange(other.has_success_, true);
			}

			return *this;
		}

		virtual ~flex_buffer() = default;

	public:
		const size_type active() const noexcept
		{
			return static_cast<size_type>(pcount_);
		}

		pointer wdata()
		{
			return buffer_.data() + static_cast<size_type>(gptr_);
		}

		const_pointer wdata() const noexcept
		{
			return buffer_.data() + static_cast<size_type>(gptr_);
		}

		pointer rdata()
		{
			return buffer_.data() + static_cast<size_type>(pptr_);
		}

		const_pointer rdata() const noexcept
		{
			return buffer_.data() + static_cast<size_type>(pptr_);
		}

		void commit(off_type bytes)
		{
			pptr_ += bytes;
			pcount_ -= bytes;
		}

		void consume(off_type bytes)
		{
			gptr_ += bytes;
		}

		size_type size() const noexcept
		{
			return static_cast<std::size_t>(pptr_ - gptr_);
		}

		void normalize()
		{
			if (pptr_ == 0)
				return;

			traits_type::copy(buffer_.data(), wdata(), size());

			pptr_ -= gptr_;
			pcount_ += gptr_;
			gptr_ = 0;
		}

		void ensure()
		{
			if (active() > water_line)
				return;

			buffer_.resize(buffer_.size() + capacity);

			pcount_ += capacity;
		}

		bool save(const value_type* data, const size_type size)
		{
			if (pcount_ < static_cast<off_type>(size))
			{
				size_type new_capacity = capacity;

				new_capacity < size ? new_capacity = size : 0;

				buffer_.resize(new_capacity + buffer_.size());

				pcount_ += new_capacity;
			}

			traits_type::copy(rdata(), data, size);

			commit(size);

			return true;
		}

		bool load(value_type* data, const size_type new_size)
		{
			if (size() < new_size)
				return false;

			traits_type::copy(data, wdata(), new_size);

			consume(new_size);

			return true;
		}

		pos_type pubseekoff(off_type off, seekdir dir, openmode mode)
		{
			pos_type new_off{};

			switch (dir)
			{
				case std::ios::beg:
				{
					new_off = 0;
				}
				break;
				case std::ios::cur:
				{
					if (mode == std::ios::in)
					{
						new_off = pptr_;
					}
					else if (mode == std::ios::out)
					{
						new_off = gptr_;
					}
				}
				break;
				case std::ios::end:
				{
					new_off = static_cast<pos_type>(buffer_.size());
				}
				break;
				default:
				return -1;
			}

			off += static_cast<off_type>(new_off);

			if (off > static_cast<off_type>(buffer_.size()) && mode == std::ios::in)
				return -1;

			if (off > pptr_ && mode == std::ios::out)
				return -1;

			if (mode == std::ios::in)
			{
				pptr_ = off;
			}

			if (mode == std::ios::out)
			{
				gptr_ = static_cast<pos_type>(off);
			}

			return off;
		}

		pos_type pubseekpos(pos_type pos, openmode mode)
		{
			switch (mode)
			{
				case std::ios::in:
				{
					if (pos > static_cast<pos_type>(buffer_.size()))
						pos = static_cast<pos_type>(buffer_.size() - 1);

					pptr_ = pos;
				}
				break;
				case std::ios::out:
				{
					if (pos > pptr_)
						return -1;

					gptr_ = pos;
				}
				break;
				default:
				pos = -1;
				break;
			}

			return pos;
		}

		bool start()
		{
			if (start_ != 0)
				return false;

			start_ = pubseekoff(0, std::ios::cur, std::ios::out);

			return true;
		}

		void close()
		{
			if (has_success_)
				return;

			pubseekpos(start_, std::ios::out);

			start_ = 0;
		}

		void failed()
		{
			has_success_ = false;
		}

		bool success() const
		{
			return has_success_;
		}

	private:
		std::vector<value_type, allocator_type> buffer_;

		pos_type pptr_;

		pos_type gptr_;

		off_type pcount_;

		pos_type start_;

		bool has_success_;
	};
}


using flex_buffer_t = detail::flex_buffer<uint8_t, std::char_traits<uint8_t>>;

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

	void swap(person& other)
	{
		std::swap(sex, other.sex);
		std::swap(addr, other.addr);
		std::swap(age, other.age);
		std::swap(telephone, other.telephone);
		std::swap(score, other.score);
		std::swap(hp, other.hp);
		std::swap(mana, other.mana);
		std::swap(info, other.info);
		std::swap(name, other.name);
		std::swap(orders, other.orders);
	}
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

	flex_buffer_t ar{};

	aquarius::binary::to(ar, p1);

	person p2 = aquarius::binary::from<person>(ar);

	BOOST_CHECK_EQUAL(p1, p2);
}

BOOST_AUTO_TEST_CASE(request)
{
	using person_request = aquarius::ip::tcp::request<person, 1001>;

	person_request req{};
	req.header()->crc32_ = 1;
	req.header()->timestamp_ = 1;
	req.body().sex = true;
	req.body().addr = 2;
	req.body().age = 15;
	req.body().telephone = 15230214856;
	req.body().score = 100;
	req.body().hp = 200;
	req.body().mana = 300;
	req.body().info = { 1, 1, 1, 1, 1, 1 };
	req.body().name = "John";
	req.body().orders = { 1, 2, 3, 4, 5 };

	flex_buffer_t ar{};

	req.to_binary(ar);

	person_request req1{};

	req1.from_binary(ar);

	BOOST_CHECK_EQUAL(req, req1);
}