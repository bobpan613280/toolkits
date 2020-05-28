#ifndef _LOGSTREAM_H_
#define _LOGSTREAM_H_
#include <assert.h>
#include <string>
#include "nocopyable.h"
#include <QString>

namespace SystemLog {
	using std::string;
	namespace detail {
		const int kSmallBuffer = 4000;
		const int kLargeBuffer = 4000 * 1000;
		template<int SIZE>
		class FixBuffer : nocopyable
		{
		public:
			FixBuffer():cur_(data_) {
				setCookie(cookieStart);
			}
			~FixBuffer() {
				setCookie(cookieEnd);
			}
			void append(const char* buf, int len)
			{
				if (avail() > len)
				{
					memcpy(cur_, buf, len);
					cur_ += len;
				}
			}
			const char* data() const { return data_; }
			int length() const { return cur_ - data_; }

			char* currren() { return cur_; }
			int avail() const { return static_cast<int>(end() - cur_); }
			void add(size_t len) { cur_ += len; }

			void reset() { cur_ = data_; }
			void bzero() {
				memset(data_, 0, sizeof(data_));
			}

			const char* debugString();
			void setCookie(void(*cookie)()) { cookie_ = cookie; }
			string asString() const { return string(data_, length()); }
		private:
			const char* end() const { return data_ + sizeof(data_); }
			static void cookieStart();
			static void cookieEnd();

			void(*cookie_)();
			char data_[SIZE];
			char* cur_;
		};
	}
	class T
	{
	public:
		T(const char* str, int len):str_(str),len_(len)
		{
			assert(strlen(str) == len_);
		}

		const char* str_;
		const size_t len_;
	};

	class LogStream :public nocopyable
	{
		typedef LogStream self;
	public:
		typedef detail::FixBuffer<detail::kSmallBuffer> Buffer;

		self& operator<<(bool v) {
			buffer_.append(v ? "1" : "0", 1);
			return *this;
		}

		self& operator<<(short);
		self& operator<<(unsigned short);
		self& operator<<(int);
		self& operator<<(unsigned int);
		self& operator<<(long);
		self& operator<<(unsigned long);
		self& operator<<(long long);
		self& operator<<(unsigned long long);
		self& operator<<(const void*);
		self& operator<<(float v) {
			*this << static_cast<double>(v);
		}
		self& operator<<(double);
		self& operator<<(char v) {
			buffer_.append(&v, 1);
			return *this;
		}
		self& operator<<(const char* v) {
			buffer_.append(v, strlen(v));
			return *this;
		}
		self& operator<<(const T& v) {
			buffer_.append(v.str_, v.len_);
			return *this;
		}
		self& operator<<(const string& v) {
			buffer_.append(v.c_str(), v.size());
			return *this;
		}
		self& operator<<(const QString& v) {
			std::string s = v.toStdString();
			buffer_.append(s.c_str(), s.size());
			return *this;
		}
		void append(const char* data, int len)
		{
			buffer_.append(data, len);
		}
		const Buffer& buffer() const {
			return buffer_;
		}
		void resetBuffer() {
			buffer_.reset();
		}
	private:
		void staticCheck();
		template<typename T>
		void formatInteger(T);
	private:
		Buffer buffer_;
		static const int KMaxNumberSize = 32;
	};
	class Fmt :nocopyable
	{
	public:
		template<typename T> 
		Fmt(const char* fmt, T val) {
			length_ = snprintf(buf_, sizeof(buf_), fmt, val);
			assert(static_cast<size_t>(length_) < sizeof buf_);
		}
		const char* data() const { return buf_; }
		int length() const {
			return length_;
		}
	private:
		char buf_[32];
		int length_;
	};
	inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
	{
		s.append(fmt.data(), fmt.length());
		return s;
	}
};


#endif // 
