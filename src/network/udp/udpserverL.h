#ifndef _UDPSERVER_L_H_
#define _UDPSERVER_L_H_
#include "udpBaseDefine.h"
#include "BaseDataDefine.h"
#include "lpSysUtils/lpSysUtils.h"
#include <mutex>
#include <functional>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>


class udpserverL
{
public:
    udpserverL(int port = 3956)
    {
        sock_fd=-1;
        _port = port;
        _pthread = 0;
        bRun = false;
        pNewUdpData._byteData = std::make_shared<ByteVector>();
    }
    virtual ~udpserverL()
    {
        bRun = false;
        if(_pthread)
        {
            _pthread->join();
            delete _pthread;
            _pthread = 0;
        }   
    }
    bool startThread();
    bool stopThread();
    void setCallbackFunc(const std::function<void(tagUdpData)> &func)
	{
		m_dataRecvCallBackFunc = func;
	}
    bool writeRaw(const tagUdpData &tagData);
    void setObjName(std::string strObj) { strObjName = strObj; };
private:
    void Maintask();
    std::function<void(tagUdpData)> m_dataRecvCallBackFunc;
    std::string strObjName;
	bool bRun;
    int sock_fd;
    int _port;
    std::thread *_pthread;

    struct sockaddr_in addr_serv;
    socklen_t len;
    ByteVector			m_tempBuf;
    tagUdpData pNewUdpData;
};

#endif
