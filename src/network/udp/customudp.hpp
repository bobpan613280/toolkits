/*!
 * \file customudp.hpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#include <cstdlib>
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
class AsyncUDP
{
public:
    AsyncUDP(const std::string & host, short port)
        : m_ioService(),
          m_socket(m_ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
          m_senderEndpoint(boost::asio::ip::udp::v4(),port),
          m_destinationEndpoint(boost::asio::ip::udp::v4(),port),
         // m_backgroundThread(),
          m_errorStatus(false),
          m_open(false)
    {
//         m_ioService.post(boost::bind(&AsyncUDP::read,this));
//         std::thread t(boost::bind(&boost::asio::io_service::run, &m_ioService));
//         m_backgroundThread.swap(t);
		std::thread m_backgroundThread(&AsyncUDP::read, this);
		m_backgroundThread.detach();
    }

    void write(const char * buf, const size_t len) {
        m_socket.async_send_to(
            boost::asio::buffer(buf, len), m_destinationEndpoint,
            std::bind(&AsyncUDP::handleSend, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void write(const std::string & str) {
        const char * buf = str.c_str();
        size_t len = str.length();
        write(buf,len);
    }
	void setDestEndpoint(boost::asio::ip::udp::endpoint point) { m_destinationEndpoint = point; }
    void doClose() {
        boost::system::error_code ec;
        m_socket.cancel(ec);
        if (ec) setErrorStatus(true);
        m_socket.close(ec);
        if (ec) setErrorStatus(true);
    }

    void close() {
        if (!get_open()) return;
        m_open = false;
        //m_backgroundThread.join();
        if (m_errorStatus) {
            throw(boost::system::system_error(boost::system::error_code(),"Error while closing the device"));
        }
    }

    void setErrorStatus(bool e) {
		//std::lock_guard<std::mutex> guard(m_udpMutex);
        m_errorStatus = e;
    }

private:

    void handleReceive(const boost::system::error_code& error,
                       std::size_t bytes_recvd)
    {
        if (!error && bytes_recvd > 0)
        {
            std::cout << "received:  " << m_readBuffer << std::endl;
        }
        else
        {
            read();
        }
    }

    void handleSend(const boost::system::error_code& error,
                    size_t bytes_sent)
    {
		if (error.value() == 0)
		{

		}
		else
		{
			std::cout << "send error" << std::endl;
		}
        //std::cout << "handle send" << std::endl;
       // read();
    }

    void read() {
        m_socket.async_receive_from(
            boost::asio::buffer(m_readBuffer, m_maxLength), m_senderEndpoint,\
            boost::bind(&AsyncUDP::handleReceive, this,\
				boost::asio::placeholders::error,\
                boost::asio::placeholders::bytes_transferred));
    }

    bool get_open() {
        return m_open;
    }

    boost::asio::io_service m_ioService;
	boost::asio::ip::udp::socket m_socket;
	boost::asio::ip::udp::endpoint m_senderEndpoint;
	boost::asio::ip::udp::endpoint m_destinationEndpoint;
    //std::thread m_backgroundThread;
    static const uint16_t m_maxLength = 1024;
    char m_readBuffer[m_maxLength];
    std::mutex m_readMutex;
    std::mutex m_writeMutex;
    bool m_errorStatus;
    std::mutex m_errorMutex;
    bool m_open;
};
/*
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
            return 1;
        }

        using namespace std; // For atoi.
        AsyncUDP s(argv[1], atoi(argv[2]));

        while(1) {
            char helloBuf[] = "buf write test";
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
            s.write("string write test");
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
            s.write(helloBuf,sizeof(helloBuf));
        };

    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}*/