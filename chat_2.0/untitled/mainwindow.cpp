#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <string.h>
#include "protocol.h"
#include <QDebug>
#include <QSize>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setIconSize(QSize(70,70));

    this->wt = WriteThread::getInstance();
    this->rt = ReadThread::getInstance();

    memset(this->buf, 0, sizeof(this->buf));
    connect(this->rt, SIGNAL(Send2UI(CHATMSG *)), this, SLOT(ShowInfo(CHATMSG*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUserData(int username)
{
    this->username = username;
}

void MainWindow::on_pushButton_clicked()
{
    const char * arr = ui->listWidget->currentItem()->text().toStdString().c_str();
    QString text = ui->textEdit_2->document()->toPlainText();

    msg.recvid = atoi(arr);
    qDebug()<<" Button_clicked msg.recipient = "<<msg.recvid;
    msg.sendid = this->username;
    
    qDebug()<<" Button_clicked msg.sender = "<<msg.sendid;
    memcpy(msg.context,text.toStdString().c_str(),sizeof(msg.context));
    qDebug()<<" Button_clicked msg.content = "<<msg.context;
    
    this->head.bussinessType = 3;
    this->head.bussinessLength = sizeof(this->msg);
    
    memcpy(this->buf,&(this->head),sizeof(this->head));
    memcpy(this->buf+sizeof(this->head),&(this->msg),sizeof(this->msg));
    
    this->wt->setData(this->buf,sizeof(this->head)+sizeof(this->msg));

    ui->textEdit_2->setPlainText("");
}

void MainWindow::ShowInfo(CHATMSG* chatmsg)
{
    qDebug()<<"ShwoInfo chatmsg->sender = "<<chatmsg->sendid;
    qDebug()<<"ShowInfo chatmsg->recipient="<<chatmsg->recvid;

    if (chatmsg->sendid == this->username)
    {
        QString send="我说:";
        QString cont = QString(chatmsg->context);
        send+=cont;
        qDebug()<<"-----"<<send<<"-----"<<endl;
        ui->textEdit->append(send);
    }else if(chatmsg->recvid == this->username){
        QString send = QString(chatmsg->sendid);
        QString mid="对我说:";
        QString cont = QString(chatmsg->context);
        send+=mid;
        send+=cont;
        ui->textEdit->append(send);
    }
    update();
}


















