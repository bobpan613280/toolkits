#include "lpLogging.h"
#include <thread>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>
#pragma execution_character_set("utf-8")
namespace SystemLog
{
	char t_errnobuf[512];
	char t_time[32];
	time_t t_lastSecond;

	lpLogging::LogLevel initLogLevel()
	{
		if (::getenv("MUDUO_LOG_TRACE"))
			return lpLogging::INFO;
		else
			return lpLogging::INFO;
	}
	lpLogging::LogLevel g_logLevel = initLogLevel();
	const char* LogLevelName[6] = {
		  "params ",
		  "user ",
		  "status  ",
		  "warn  ",
		  "info ",
		  "num ",
	};
	void defaultOutput(int nTypeID, const char* msg, int len)
	{
		size_t n = fwrite(msg, 1, len, stdout);
		(void)n;
	}
	void defaultFlush()
	{
		fflush(stdout);
	}
	lpLogging::OutputFunc g_output = defaultOutput;
	lpLogging::FlushFunc g_flush = defaultFlush;

	lpLogging::Impl::Impl(LogLevel level, int savedErrno)
		:stream_(), level_(level)
	{
		//formatTime();
	}

	void lpLogging::Impl::formatTime()
	{
		time_t seconds = 0;
		time(&seconds);
		int microseconds = seconds % 1000000;
		if (seconds != t_lastSecond)
		{
			t_lastSecond = seconds;
			struct tm tm_time;
			tm_time = *localtime(&seconds);

			int len = snprintf(t_time, sizeof(t_time), "%4d-%02d-%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
			assert(len == 19); (void)len;
		}

		stream_ << T(t_time, 19) << "\n";
	}

	void lpLogging::Impl::finish()
	{
		stream_ << '\n';
	} 
	lpLogging::lpLogging()
		:impl_(INFO,0)
	{

	}
	lpLogging::lpLogging(LogLevel level, const char* func)
		: impl_(level, 0) 
	{
		impl_.stream_ << func << " ";
	}
	lpLogging::lpLogging(LogLevel level)
		: impl_(level, 0)
	{
	}
	lpLogging::LogLevel lpLogging::logLevel()
	{
		return g_logLevel;
	}
	void lpLogging::setLogLevel(LogLevel level)
	{
		g_logLevel = level;
	}
	void lpLogging::setOutput(OutputFunc out)
	{
		g_output = out;
	}
	void lpLogging::setFlush(FlushFunc flush)
	{
		g_flush = flush;
	}
}

using namespace SystemLog;
using namespace SystemLog::detail;
lpLogging::~lpLogging() {
	impl_.finish();
	const LogStream::Buffer& buf(stream().buffer());
	g_output(impl_.level_, buf.data(), buf.length());
// 	if (impl_.level_ == FATAL)
// 	{
// 		g_flush();
// 		abort();
// 	}
}