#include "socketconnect.h"
#include <arpa/inet.h>

//lazy man singleton
SocketConnect * SocketConnect::sc = nullptr;
SocketConnect * SocketConnect::getInstance()
{
    if(SocketConnect::sc == nullptr)
    {
        SocketConnect::sc = new SocketConnect();
    }
    return SocketConnect::sc;
}
//hungry man Singleton
/*
SocketConnect * SocketConnect::sc = new SocketConnect();
SocketConnect * SocketConnect::getInstance()
{
    return SocketConnect::sc;
}
*/

SocketConnect::SocketConnect()
{
    this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    this->len = sizeof(s_addr);
}


void SocketConnect::CreateConnect()
{
    if(socketFd == -1)
    {
        perror("socket error");
    }else
    {
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(10001);
        s_addr.sin_addr.s_addr = inet_addr("192.168.177.130");

        len = sizeof(s_addr);
        if(::connect(socketFd, (struct sockaddr*)&s_addr, len) == -1)
        {
            perror("connect error");
        }
    }
}

void SocketConnect::Close()
{
    if(socketFd == -1)
    {
        perror("socket error");
    }
    else{
        close(socketFd);
    }
}

int SocketConnect::getSocketFd() const
{
    return socketFd;
}

void SocketConnect::setSocketFd(int value)
{
    socketFd = value;
}
