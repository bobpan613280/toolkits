#include "udpserverL.h"
int main()
{
    udpserverL server(8000);
    server.startThread();
    while(1);
}