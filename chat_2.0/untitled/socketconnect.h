#ifndef SOCKETCONNECT_H
#define SOCKETCONNECT_H

#include <QObject>
#include <QWidget>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <QTcpSocket>
#include <netinet/in.h>
#include <QDebug>

class SocketConnect : public QObject
{
    Q_OBJECT

public:
    static SocketConnect *sc;
    static SocketConnect *getInstance();

    void CreateConnect();
    void Close();
    int getSocketFd() const;
    void setSocketFd(int value);

private:
    SocketConnect();
    struct sockaddr_in s_addr;
    int socketFd;
    int len;
};

#endif // SOCKETCONNECT_H
