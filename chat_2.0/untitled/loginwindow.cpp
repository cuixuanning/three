#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <string.h>

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    this->sc = SocketConnect::getInstance();
    this->sc->CreateConnect();
    this->rt = ReadThread::getInstance();
    this->wt = WriteThread::getInstance();

    this->rt->setSocketFd(this->sc->getSocketFd());
    this->wt->setSocketFd(this->sc->getSocketFd());

    memset(this->buf, 0, sizeof(this->buf));

    connect(this->rt, SIGNAL(Send2SigninWidget(int)),
            this, SLOT(isChecked(int)));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_signinBtn_clicked()
{
    QString account = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    if(account.length()>0 && password.length()>0)
    {
//        const char* pname = account.toStdString().c_str();
//        const char* ppass = password.toStdString().c_str();

//        user.name = atoi(pname);
//        memcpy(user.pass, ppass, sizeof(user.pass));
        std::string nameStr = account.toStdString();
        std::string passStr = password.toStdString();

        user.name = atoi(nameStr.c_str());
        memset(user.pass, 0, sizeof(user.pass));
        memcpy(user.pass, passStr.c_str(), passStr.size());

        qDebug()<<"user.username"<<user.name;
        qDebug()<<"user.password"<<user.pass;
        qDebug()<<"sizeof(head.businesslen = "<<sizeof(this->user);

        this->pro.bussinessType = 1;
        this->pro.bussinessLength = sizeof(this->user);

        memset(this->buf, 0, sizeof(this->buf));
        memcpy(this->buf, &(this->pro), sizeof(this->pro));
        memcpy(this->buf + sizeof(this->pro), &(this->user), sizeof(this->user));

        this->rt->start();

        this->wt->setData(this->buf, sizeof(this->pro)+sizeof(this->user));

        this->wt->start();

    }
}

void LoginWindow::on_cencleBtn_clicked()
{
    this->sc->Close();
    this->close();
}

void LoginWindow::isChecked(int flag)
{
    if(flag == 1)
    {
        QMessageBox::information(NULL, "title", "login success");

        this->charWin = new MainWindow();
        this->charWin->setUserData(user.name);

        this->hide();
        this->charWin->show();
    }
    else {
        QMessageBox::information(NULL, "title", "login error,already login");
    }
}
