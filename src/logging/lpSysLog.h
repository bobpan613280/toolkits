#ifndef _LPSYSLOG_H_
#define _LPSYSLOG_H_
#include <QObject>
#include "lp_singleton_sys.h"
#include "lpLogging.h"
#include "lpLogDB.h"
#include <QStringList>

namespace SystemLog {
	enum CheckType {
		EM_SYS_PARAM = 0,//���� �޸ļ�¼
		EM_SYS_USER = 1,//�û���¼��¼
		EM_SYS_STATUS = 2,//ϵͳ״̬��¼
		EM_SYS_WARN = 3,//������¼
		EM_SYS_INFO = 4,//��Ϣ ����
	};

	class lpSysLog :public QObject, public lp_singleton_sys<lpSysLog>
	{
		Q_OBJECT
	public:
		lpSysLog();
		~lpSysLog();

		void Init();
		void Release();
		SysLogMsgList checkByDate(QString strDate, int type);
		SysLogMsgList checkByDateTime(QString startTime, QString endTime, int type);
	private:
		SystemLog::lpLogDB m_DB;
	};
};
using namespace SystemLog;
#endif
