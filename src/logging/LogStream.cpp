#include "LogStream.h"
#pragma execution_character_set("utf-8")
namespace SystemLog
{
	namespace detail 
	{
		const char digits[]= "9876543210123456789";
		const char* zero = digits + 9;
		const char digitsHex[] = "0123456789abcdef";

		template<int SIZE>
		const char* FixBuffer<SIZE>::debugString()
		{
			*cur_ = '\0';
			return data_;
		}
		template<int SIZE>
		void FixBuffer<SIZE>::cookieEnd()
		{

		}
		template<int SIZE>
		void FixBuffer<SIZE>::cookieStart()
		{

		}
		template class FixBuffer<kSmallBuffer>;
		template class FixBuffer<kLargeBuffer>;

		template<typename T>
		size_t convert(char buf[], T value)
		{
			T i = value;
			char* p = buf;
			do {
				int lsd = i % 10;
				i /= 10;
				*p++ = zero[lsd];
			} while (i != 0);
			if (value < 0) {
				*p++ = '-';
			}
			*p = '\0';
			std::reverse(buf, p);
			return (p - buf);
		}

		size_t convertHex(char buf[], uintptr_t value) {
			uintptr_t i = value;
			char* p = buf;
			do {
				int lsd = i % 16;
				i /= 16;
				*p++ = digitsHex[lsd];
			} while (i != 0);

			*p = '\0';
			std::reverse(buf, p);
			return (p - buf);
			{

			}
		}
	}


	template<typename T>
	void LogStream::formatInteger(T v)
	{
		if (buffer_.avail() >= KMaxNumberSize)
		{
			size_t len = detail::convert(buffer_.currren(), v);
			buffer_.add(len);
		}
	}

	LogStream& LogStream::operator<<(short v)
	{
		*this << static_cast<int>(v);
		return *this;
	}
	LogStream& LogStream::operator<<(unsigned short v)
	{
		*this << static_cast<unsigned int>(v);
		return *this;
	}
	LogStream& LogStream::operator<<(int v)
	{
		formatInteger(v);
		return *this;
	}
	LogStream& LogStream::operator<<(unsigned int v)
	{
		formatInteger(v);
		return *this;
	}
	LogStream& LogStream::operator<<(long v)
	{
		formatInteger(v);
		return *this;
	}
	LogStream& LogStream::operator<<(unsigned long v)
	{
		formatInteger(v);
		return *this;
	}
	LogStream& LogStream::operator<<(long long v)
	{
		formatInteger(v);
		return *this;
	}
	LogStream& LogStream::operator<<(unsigned long long v)
	{
		formatInteger(v);
		return *this;
	}
	LogStream& LogStream::operator<<(const void* p) 
	{
		uintptr_t v = reinterpret_cast<uintptr_t>(p);
		if (buffer_.avail() >= KMaxNumberSize) {
			char* buf = buffer_.currren();
			buf[0] = '0';
			buf[1] = 'x';
			size_t len = detail::convertHex(buf + 2, v);
			buffer_.add(len + 2);
		}
		return *this;
	}
	LogStream& LogStream::operator<<(double v)
	{
		if(buffer_.avail() >= KMaxNumberSize) {
			int len = snprintf(buffer_.currren(), KMaxNumberSize, "%.12g", v);
			buffer_.add(len);
		}
		return *this;
	}
	void LogStream::staticCheck()
	{
		assert(KMaxNumberSize - 10 > std::numeric_limits<long long>::digits10);
		assert(KMaxNumberSize - 10 > std::numeric_limits<double>::digits10);
		assert(KMaxNumberSize - 10 > std::numeric_limits<long double>::digits10);
		assert(KMaxNumberSize - 10 > std::numeric_limits<long>::digits10);
		assert(KMaxNumberSize - 10 > std::numeric_limits<long long>::digits10);
	}
}

// Explicit instantiations
using namespace SystemLog;

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);

