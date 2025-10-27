#include "readthread.h"

ReadThread *ReadThread::readThread = nullptr;

ReadThread *ReadThread::getInstance()
{
    if(ReadThread::readThread == nullptr)
    {
        ReadThread::readThread = new ReadThread();
    }
    return ReadThread::readThread;
}

ReadThread::ReadThread()
{

}

void ReadThread::run()
{
    int res = 0;
    while(1)
    {
        read(this->socketFd, this->buf, sizeof(HEAD));
        memcpy(&this->head, this->buf, sizeof(HEAD));
        qDebug()<<this->head.bussinessType;
        bzero(this->buf, sizeof(this->buf));

        if(this->head.bussinessType == 2)
        {
            res = read(this->socketFd, this->buf, sizeof(LOGINBACK));
            memcpy(&this->back, this->buf, sizeof(LOGINBACK));
            qDebug()<<"flag = "<<back.flag;
            emit Send2SigninWidget(back.flag);
        }else if(this->head.bussinessType == 4)
        {
            res = read(this->socketFd, this->buf, sizeof(CHATMSG));
            memcpy(&this->chatmsg, this->buf, sizeof(CHATMSG));
            qDebug()<<"run charmsg->sender = "<<chatmsg.sendid;
            qDebug()<<"run charmsg->recvid = "<<chatmsg.recvid;
            qDebug()<<"run charmsg->context = "<<chatmsg.context;
            emit Send2UI(&this->chatmsg);
        }
    }
}

void ReadThread::setSocketFd(int value)
{
    this->socketFd = value;
}

