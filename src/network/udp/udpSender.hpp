/*!
 * \file udpSender.hpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _UDPSENDER_H_
#define _UDPSENDER_H_
#include "udpBaseDefine.h"
#include "BaseDataDefine.h"
#include <boost/asio.hpp>
#include <lpSysUtils/lpSysUtils.h>
class udpSender
{
public:
	udpSender(std::string addr="*", int port=0):m_pSocket(0)
	{
		if(addr=="*")
			//_LocalPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("192.168.8.153"), port);
			_LocalPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
		else
			_LocalPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(addr), port);
	}
	virtual ~udpSender()
	{
		if (m_pSocket) {
			m_pSocket->close();
			delete m_pSocket;
			m_pSocket = NULL;
		}
	}
	bool Start()
	{
		try {
			if (m_pSocket) {
				std::cout<<"udp sender socket is not null"<< std::endl;
				m_pSocket->close();
				delete m_pSocket;
			}
			std::cout<<"udpsender ip:"<<_LocalPoint.address().to_string()<<" port:"<<_LocalPoint.port()<< std::endl;
			m_pSocket = new boost::asio::ip::udp::socket(m_pios, _LocalPoint);
			std::cout << "udpSender create OK" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << "udpSender create error!!!" << std::endl;
			return false;
		}
		m_pios.run();
		return true;
	}
	void stop()
	{
		m_pios.stop();
	}
	bool writeRaw(const tagUdpData &tagData)
	{
		boost::shared_ptr<std::string> strMessage(new std::string((char*)(tagData._byteData->data())));
		const lp_uint8 *pBuffData = tagData._byteData->data();
		std::size_t sendSize = tagData._byteData->size();
		std::string sendMsg;
		boost::asio::ip::udp::endpoint targetpoint(boost::asio::ip::address::from_string(lpTools::ip2string((tagData.sin_addr)).c_str()),(tagData.sin_port));
		
		if (sendSize < 1400)
			std::cout << "gvsp ip :" << targetpoint.address().to_string() << " port :" << targetpoint.port() << std::endl;
		try {
			if (m_pSocket)
			{
				std::size_t s = m_pSocket->send_to(boost::asio::buffer(pBuffData, sendSize), targetpoint);
			}
		}
		catch (std::exception &e) {
			std::cout << "IP:" << tagData.sin_addr << " port:" << tagData.sin_port << std::endl;
			std::cout << "sender data error" << std::endl;
		}
		return true;
	}
private:
	boost::asio::ip::udp::socket *m_pSocket;
	boost::asio::ip::udp::endpoint _LocalPoint;
	boost::asio::io_service m_pios;
};

#endif
