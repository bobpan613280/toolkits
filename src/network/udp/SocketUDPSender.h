/*!
 * \file SocketUDPSender.h
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _SOCKETUDPSENDER_H_
#define _SOCKETUDPSENDER_H_
#include "BaseDataDefine.h"
#include "udpBaseDefine.h"

#ifndef WINDOWS
#include <sys/types.h>
#include <error.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>

class SocketUDPSender{
public:
    SocketUDPSender(std::string addr = "*",int port =0);
    SocketUDPSender(int port=0);
    virtual ~SocketUDPSender();
    virtual bool Start();
    
    bool writeRaw(const tagUdpData & tagData);

private:
    int sock_fd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t clilen;
    int bd;
    int localport;
};
#endif
#endif
