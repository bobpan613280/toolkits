#include "udpserverL.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
bool udpserverL::startThread()
{
    sock_fd = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_fd<0)
        return false;
    
    //struct sockaddr_in addr_serv;
    

    _pthread = new std::thread(&udpserverL::Maintask,this);

    return true;
}
bool udpserverL::stopThread()
{
    bRun = false;
    return true;
}

bool udpserverL::writeRaw(const tagUdpData &tagData)
{
    struct sockaddr_in addr_client;
    memset(&addr_client,0,sizeof(addr_client));
    addr_client.sin_family = AF_INET;
    //int clientPort = ;
    addr_client.sin_port = tagData.sin_port;//htons(clientPort);
    addr_client.sin_addr.s_addr = tagData.sin_addr;//htonl();
    len = sizeof(addr_client);
    
    const lp_uint8 *pBuffData = tagData._byteData->data();
    std::size_t sendSize = tagData._byteData->size();
   // std::cout<<"send "<<lpTools::ip2string(tagData.sin_addr)<<std::endl;
    size_t send_num = sendto(sock_fd,pBuffData,sendSize,0,(struct sockaddr*)&addr_client,len);
    if(send_num < 0)
    {
        perror("sendto error:"); 
        exit(1);
    }

    return true;
}

void udpserverL::Maintask()
{
    memset(&addr_serv,0,sizeof(struct sockaddr_in));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(_port);
    addr_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    len = sizeof(addr_serv);
//std::cout<<"Maintask"<<std::endl;
    if(bind(sock_fd,(struct sockaddr*)&addr_serv,sizeof(addr_serv))<0)
    {
        perror("bind error:");
        return ;
    }
    bRun = true;

    int recv_num;
    int send_num;
    //char send_buf[1024]="i am server";
    char recv_buf[1024];
    struct sockaddr_in addr_client;
    while(bRun)
    {
       // printf("server wait\n");
        recv_num = recvfrom(sock_fd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&addr_client,(socklen_t *)&len);
        if(recv_num<0)
        {
            perror("recvfrom error:");
            break;

        }
        
        pNewUdpData._byteData->resize(recv_num,0);
        uint8_t *ptr = (*pNewUdpData._byteData).data();
        memcpy(ptr,recv_buf,recv_num);

        pNewUdpData.sin_addr = /*ntohl*/(addr_client.sin_addr.s_addr);
        pNewUdpData.sin_port = /*ntohs*/(addr_client.sin_port);
       // std::cout<<"recv "<<lpTools::ip2string(pNewUdpData.sin_addr)<<"port "<<pNewUdpData.sin_port<<std::endl;
       // printf("recv message\n");
        
        if(m_dataRecvCallBackFunc)
            m_dataRecvCallBackFunc(pNewUdpData);

    }
    close(sock_fd); 
}