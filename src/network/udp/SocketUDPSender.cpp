/*!
 * \file SocketUDPSender.cpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#include "SocketUDPSender.h"
#ifndef WINDOWS
SocketUDPSender:: SocketUDPSender(std::string addr,int port):sock_fd(-1)
{
    localport = port;
}
SocketUDPSender::SocketUDPSender(int port) : sock_fd(-1)
{
    localport = port;
}

SocketUDPSender::~SocketUDPSender()
{
    close(sock_fd);
}

bool SocketUDPSender::Start()
{
    sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_fd<0){
        std::cout<<"socket init error"<<std::endl;
        return false;
    }
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    server.sin_family = AF_INET;
    server.sin_port = htons(localport);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bd = bind(sock_fd, (struct sockaddr*)&server, sizeof(server));
    if(bd <0){
        std::cout<<"bind error"<<std::endl;
        return false;
    }
    return true;
}

bool SocketUDPSender::writeRaw(const tagUdpData & tagData)
{
    memset(&client,0,sizeof(client));
    client.sin_family = AF_INET;
   // std::string strClientIp;// = 
    client.sin_port = htons(tagData.sin_port);
    client.sin_addr.s_addr = htonl(tagData.sin_addr);
    //inet_pton(AF_INET,strClientIp.c_str(),&client.sin_addr);
    clilen = sizeof(client);
    
    const lp_uint8 *pBuffData = tagData._byteData->data();
    std::size_t sendSize = tagData._byteData->size();
   // std::cout<<"size:"<<sendSize<<std::endl;
    sendto(sock_fd,pBuffData,sendSize,0,(struct sockaddr*)&client,clilen);
    return true;
}
#endif