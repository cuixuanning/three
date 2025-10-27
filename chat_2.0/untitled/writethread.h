#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <QDebug>
#include <string.h>
#include <unistd.h>

class WriteThread : public QThread
{
    Q_OBJECT

public:
    static WriteThread * writeThread;
    static WriteThread * getInstance();
    void run();
    void setData(void *value, int size);
    void setSocketFd(int value);

private:
    WriteThread();
    int socketFd;
    char data[1024];
    int length;
};

#endif // WRITETHREAD_H
