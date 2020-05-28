/*!
 * \file udpServer.hpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _UDPSERVER_H_
#define _UDPSERVER_H_
#include "udpBaseDefine.h"
#include "BaseDataDefine.h"
#include <mutex>
#include <functional>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <lpSysUtils/lpSysUtils.h>

static void DefaultTagfunc(tagUdpData pByV)
{
}
class udpServer
{
public:
	udpServer(std::string addr = "*",int port=0):m_pSocket(0)
	{
		//if(addr == "*")
			_serverPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
		//else
		//	_serverPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(addr.c_str()), port);
		std::cout << "udp serverip :" << _serverPoint.address().to_string() << " port:" << _serverPoint.port() << std::endl;
		bRun = true;
		m_timeOut = 1000;
		m_bufSize = 1024;
		m_dataRecvCallBackFunc = DefaultTagfunc;
		m_tempBuf.resize(m_bufSize, 0);//1024bytes
		pNewUdpData._byteData = std::make_shared<ByteVector>();
	}
	virtual ~udpServer()
	{
		if (m_pSocket) {
			m_pSocket->close();
			delete m_pSocket;
			m_pSocket = NULL;
		}
		bRun = false;
		m_pios.stop();
		std::cout << "udpserver Object deleted!" << std::endl;
	}

	void setTimeOut(int timeout = 3000)
	{
		m_timeOut = timeout;
	}
	void setReadBuffSize(int nSize = 1024)
	{
		m_bufSize = nSize;
		m_tempBuf.resize(m_bufSize, 0);
	}
	bool startThread()
	{
		try {
			if (m_pSocket) {
				std::cout<<"udp server socket is not null"<< std::endl;
				m_pSocket->close();
				delete m_pSocket;
			}
			std::cout<<"udp server IP:"<<_serverPoint.address()<<" Port: "<<_serverPoint.port()<< std::endl;
			m_pSocket = new boost::asio::ip::udp::socket(m_pios, _serverPoint);
			std::cout << " UDP server create socket OK" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << " UDP server Failed to create socket !" << std::endl;
			return false;
		}

		try{
			bRun = true;
			m_thread = std::thread(std::bind(&udpServer::mainRun, this));
			std::cout << "success to create udp thread" << std::endl;
			m_thread.detach();
		}
		catch(std::exception &e) {
			std::cout << "Failed to create thread !" << std::endl;
			return false;
		}
		return true;
	}

	bool stopThread()
	{
		m_pios.stop();
		bRun = false;
		return true;
	}

	void setCallbackFunc(const std::function<void(tagUdpData)> &func)
	{
		m_dataRecvCallBackFunc = func;
	}
	/*example:setCallbackFunc(std::bind(&boostSeriport::serialCallback,this,_1));*/

	bool writeRaw(const tagUdpData &tagData)
	{
		boost::shared_ptr<std::string> strMessage(new std::string((char*)(tagData._byteData->data())));
		const uint8_t * p = tagData._byteData->data();
		std::size_t buffsize = tagData._byteData->size();
		std::string sendMsg;
		boost::asio::ip::udp::endpoint targetpoint(boost::asio::ip::address::from_string(lpTools::ip2string(htonl(tagData.sin_addr)).c_str()),tagData.sin_port);
		try {
			if (m_pSocket) {
				m_pSocket->async_send_to(boost::asio::buffer(p, buffsize), targetpoint,
					boost::bind(&udpServer::writeHandler, this, strMessage,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}
		catch (std::exception &e) {
			std::cout << "send data error,SendDataOnce" << std::endl;
		}

		return true;
	}
	void setObjName(std::string strObj) { strObjName = strObj; };

private:
	void mainRun()
	{
		std::cout << "udp server::mainRun()"<< std::endl;
		readDataOnce();
		m_pios.run();
	}
	void readDataOnce()//read data from buff
	{
		try {
			if (m_pSocket) {
				m_pSocket->async_receive_from(boost::asio::buffer(m_tempBuf), _endpointRemote,
					boost::bind(&udpServer::readHandler, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
			}
		}
		catch (std::exception &e) {
			std::cout << "read data error,readDataOnce" << std::endl;
		}
	}
	void readHandler(const boost::system::error_code &ec, std::size_t bytesTransferred)
	{
		if (ec) {
			std::cout << "udp read error!!" << std::endl;
			return;
		}
		pNewUdpData._byteData->resize(bytesTransferred, 0);
		uint8_t * p = m_tempBuf.data();
		uint8_t *pdst = (*(pNewUdpData._byteData)).data();
		memcpy(pdst, p, bytesTransferred);
		pNewUdpData.sin_port = (_endpointRemote.port());
		pNewUdpData.sin_addr = (lpTools::string2ip(_endpointRemote.address().to_string()));
		if (m_dataRecvCallBackFunc)//
			m_dataRecvCallBackFunc(pNewUdpData);
		readDataOnce();
	}
	void writeHandler(boost::shared_ptr<std::string> str, const boost::system::error_code& ec, std::size_t s)
	{
		//std::cout<<"finish\n";
		//if(!ec)
		//{
		//	std::cout << "Timeout!!" <<std::endl;
		//}
	}
	void timeoutHandler(const boost::system::error_code &ec)
	{
		if (!ec) {
			std::cout << "Timeout!!" <<std::endl;
		}
	}
private:
	tagUdpData pNewUdpData;//提取数据和回调时使用
	boost::asio::io_service m_pios;// 
	int					m_timeOut;//
	int					m_bufSize;//read buf
	ByteVector			m_tempBuf;//

	std::function<void(tagUdpData)> m_dataRecvCallBackFunc;//
	std::function<void()> m_errorCallbackFunc;//

	std::thread m_thread;//io_service::run
	boost::asio::ip::udp::socket *m_pSocket;
	boost::asio::ip::udp::endpoint _endpointRemote;//
	boost::asio::ip::udp::endpoint _serverPoint;
	std::string strObjName;
	bool bRun;
};

#endif // _UDPSERVER_H_

