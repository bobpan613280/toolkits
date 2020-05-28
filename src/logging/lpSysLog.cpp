#include "lpSysLog.h"
namespace SystemLog {
	lpSysLog::lpSysLog()
	{
	}


	lpSysLog::~lpSysLog()
	{
		Release();
	}

	void lpSysLog::Init()
	{
		SystemLog::lpLogging::OutputFunc calbackfunc = std::bind(&SystemLog::lpLogDB::onRecvData, &m_DB, \
			std::placeholders::_1, \
			std::placeholders::_2, \
			std::placeholders::_3);
		SystemLog::lpLogging::setOutput(calbackfunc);
		m_DB.startProcess();
	}

	void lpSysLog::Release()
	{
		m_DB.stopProcess();
	}

	SysLogMsgList lpSysLog::checkByDate(QString strDate, int type)
	{
		return m_DB.checkByDate(strDate,type);
	}
	SysLogMsgList lpSysLog::checkByDateTime(QString startTime, QString endTime, int type)
	{
		return m_DB.checkByDateTime(startTime, endTime, type);
	}
}