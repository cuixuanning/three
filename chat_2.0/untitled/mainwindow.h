#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "writethread.h"
#include "readthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUserData(int username);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    WriteThread* wt;
    ReadThread* rt;
    CHATMSG msg;
    HEAD head;
    char buf[2000];
    int username;

public slots:
    void ShowInfo(CHATMSG* chatmsg);
};

#endif // MAINWINDOW_H
