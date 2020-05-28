#ifndef _QLOGGING_H_
#define _QLOGGING_H_
#include "LogStream.h"
#include <functional>
namespace SystemLog {
	class lpLogging
	{
	public:
		enum LogLevel {
			PARAM = 0,//���� �޸ļ�¼
			USER = 1,//�û���¼��¼
			STATUS = 2,//ϵͳ״̬��¼
			WARN = 3,//������¼
			INFO = 4,//��Ϣ ����
			NUM_LOG_LEVELS,//����
		};
		lpLogging();
		lpLogging(LogLevel level);
		lpLogging(LogLevel level, const char* func);
		~lpLogging();
		
		LogStream& stream() 
		{ 
			return impl_.stream_;
		}
		static LogLevel logLevel();
		static void setLogLevel(LogLevel level);

		typedef std::function<void(int ntypeID, const char* msg, int len)> OutputFunc;
		typedef std::function<void()> FlushFunc;
		static void setOutput(OutputFunc);
		static void setFlush(FlushFunc);
	private:
		class Impl {
		public:
			typedef lpLogging::LogLevel LogLevel;
			Impl(LogLevel level, int old_errno);
			void formatTime();
			void finish();
			LogStream stream_;
			LogLevel level_;
		};
		Impl impl_;
	};
}

#define SYSLOG_PARAM	SystemLog::lpLogging(SystemLog::lpLogging::PARAM).stream()
#define SYSLOG_USER		SystemLog::lpLogging(SystemLog::lpLogging::USER).stream()
#define SYSLOG_STATUS	SystemLog::lpLogging(SystemLog::lpLogging::STATUS).stream()
#define SYSLOG_WARN		SystemLog::lpLogging(SystemLog::lpLogging::WARN).stream()
#define SYSLOG_INFO		SystemLog::lpLogging(SystemLog::lpLogging::INFO).stream()

#endif
