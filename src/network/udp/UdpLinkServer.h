#ifndef _UDPLINKSERVER_H_
#define _UDPLINKSERVER_H_

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
using boost::asio::ip::udp;

#define  UDP_DATA_PACKAGE_MAX_LENGTH		1024

//发送数据回调函数
typedef void (CALLBACK *SendDataCallback)(const boost::system::error_code& error, std::size_t bytes_transferred, DWORD dwUserData1, DWORD dwUserData2);
//接收数据回调函数
typedef void (CALLBACK *RecvDataCallback)(const boost::system::error_code& error, char *pData, int nDataLength, char *pPeerIp, unsigned short usPeerPort, DWORD dwUserData1, DWORD dwUserData2);

class UdpLinkServer
{
public:
	UdpLinkServer(unsigned short usPort, bool bBroadcast);
	virtual ~UdpLinkServer(void);

	//发送数据回调函数 boost function定义
	typedef boost::function<void* (const boost::system::error_code& error, std::size_t bytes_transferred, DWORD dwUserData1, DWORD dwUserData2)>	SendDataCallbackHandler;

	/*
	功能：设置接收数据回调函数
	参数：bAutoRecvData：是否自动接收数据；pfunc：接收数据回调函数；dwUserData1：接收数据回调函数用户数据1；dwUserData2：接收数据回调函数用户数据2
	返回值：无
	*/
	void SetRecvDataCallback(bool bAutoRecvData, RecvDataCallback pfunc, DWORD dwUserData1, DWORD dwUserData2);

	//开始
	int Start(boost::asio::io_service& ioService);

	//停止
	int Stop();

	//发送数据
	int SendDataEx(udp::endpoint endpointRemote, char *pBuffer, int nBufferSize, SendDataCallback pfunc, DWORD dwUserData1, DWORD dwUserData2);

	//发送数据
	int SendData(char *pBuffer, int nBufferSize, bool bAsync);

	//获取广播端点
	udp::endpoint & GetBroadcastEndPoint();

	//接收数据处理（手动）
	void handleRecvDataByManual(RecvDataCallback pfunc = NULL, DWORD dwUserData1 = 0, DWORD dwUserData2 = 0);
	//当发送数据给客户端成功之后响应处理
	void handleSendData(char *pBuffer, int nBufferSize, const boost::system::error_code& error, std::size_t bytes_transferred);
	void handleSendDataInner(SendDataCallback pfunc, DWORD dwUserData1, DWORD dwUserData2, const boost::system::error_code& error, std::size_t bytes_transferred);
	//void handleSendData(boost::shared_ptr<std::string> strMessage,const boost::system::error_code& error,std::size_t bytes_transferred);

	static void WINAPI SendDataCallbackOuter(const boost::system::error_code& error, std::size_t bytes_transferred, DWORD dwUserData1, DWORD dwUserData2);


protected:
	//接收数据
	void RecvDataProcess(RecvDataCallback pfunc, DWORD dwUserData1, DWORD dwUserData2);
	//接收数据处理（自动）
	void handleRecvData(const boost::system::error_code& error, std::size_t bytes_transferred, RecvDataCallback pfunc, DWORD dwUserData1, DWORD dwUserData2);

	//是否停止服务
	bool IsStop();

private:
	udp::socket	*m_sockUdp;										//服务器的SOCKET
	udp::endpoint m_endpointRemote;								//收到数据时的端点信息
	udp::endpoint m_endpointBroadcast;							//广播端点信息
	boost::array<char, UDP_DATA_PACKAGE_MAX_LENGTH> m_recvBuf;	//接收数据缓冲区
	bool m_bStop;												//停止服务
	bool  m_bBroadcast;											//是否广播
	unsigned short m_usPort;									//端口
	bool m_bAutoRecvData;										//是否自动接收数据，true，表示自动接收数据；false，表示外部手动调用函数接收数据
	RecvDataCallback											m_pfuncRecvDataCallback;		//接收数据回调函数
	DWORD														m_dwRecvDataCallbackUserData1;	//接收数据回调函数用户数据1
	DWORD														m_dwRecvDataCallbackUserData2;  //接收数据回调函数用户数据2
};
#endif