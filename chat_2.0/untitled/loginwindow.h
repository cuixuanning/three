#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "readthread.h"
#include "writethread.h"
#include "socketconnect.h"
#include "protocol.h"
#include <mainwindow.h>
#include <QMessageBox>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_signinBtn_clicked();
    void on_cencleBtn_clicked();

private:
    Ui::LoginWindow *ui;
    SocketConnect* sc;
    ReadThread* rt;
    WriteThread* wt;
    HEAD pro;
    USER user;
    char buf[2000];
    MainWindow* charWin;

public slots:
    void isChecked(int flag);

};

#endif // LOGINWINDOW_H
