/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_start;
    QPushButton *pushButton_pause;
    QPushButton *pushButton_continue;
    QPushButton *pushButton_stop;
    QPushButton *pushButton_startRecord;
    QPushButton *pushButton_stopRecord;
    QLabel *label_frameInfo;
    QLabel *label_video;
    QSlider *horizontalSlider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1200, 850);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton_start = new QPushButton(centralWidget);
        pushButton_start->setObjectName(QStringLiteral("pushButton_start"));
        pushButton_start->setGeometry(QRect(30, 700, 120, 50));
        QFont font;
        font.setPointSize(15);
        pushButton_start->setFont(font);
        pushButton_start->setIconSize(QSize(12, 12));
        pushButton_pause = new QPushButton(centralWidget);
        pushButton_pause->setObjectName(QStringLiteral("pushButton_pause"));
        pushButton_pause->setGeometry(QRect(200, 700, 120, 50));
        pushButton_pause->setFont(font);
        pushButton_pause->setIconSize(QSize(12, 12));
        pushButton_continue = new QPushButton(centralWidget);
        pushButton_continue->setObjectName(QStringLiteral("pushButton_continue"));
        pushButton_continue->setGeometry(QRect(340, 700, 120, 50));
        pushButton_continue->setFont(font);
        pushButton_continue->setIconSize(QSize(12, 12));
        pushButton_stop = new QPushButton(centralWidget);
        pushButton_stop->setObjectName(QStringLiteral("pushButton_stop"));
        pushButton_stop->setGeometry(QRect(480, 700, 120, 50));
        pushButton_stop->setFont(font);
        pushButton_stop->setIconSize(QSize(12, 12));
        pushButton_startRecord = new QPushButton(centralWidget);
        pushButton_startRecord->setObjectName(QStringLiteral("pushButton_startRecord"));
        pushButton_startRecord->setGeometry(QRect(620, 700, 120, 50));
        pushButton_startRecord->setFont(font);
        pushButton_startRecord->setIconSize(QSize(12, 12));
        pushButton_stopRecord = new QPushButton(centralWidget);
        pushButton_stopRecord->setObjectName(QStringLiteral("pushButton_stopRecord"));
        pushButton_stopRecord->setGeometry(QRect(760, 700, 120, 50));
        pushButton_stopRecord->setFont(font);
        pushButton_stopRecord->setIconSize(QSize(12, 12));
        label_frameInfo = new QLabel(centralWidget);
        label_frameInfo->setObjectName(QStringLiteral("label_frameInfo"));
        label_frameInfo->setGeometry(QRect(50, 600, 330, 30));
        QFont font1;
        font1.setPointSize(12);
        label_frameInfo->setFont(font1);
        label_video = new QLabel(centralWidget);
        label_video->setObjectName(QStringLiteral("label_video"));
        label_video->setGeometry(QRect(10, 10, 1180, 550));
        label_video->setAlignment(Qt::AlignCenter);
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(50, 650, 1100, 20));
        horizontalSlider->setOrientation(Qt::Horizontal);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 17));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        pushButton_start->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\346\222\255\346\224\276", Q_NULLPTR));
        pushButton_pause->setText(QApplication::translate("MainWindow", "\346\232\202\345\201\234\346\222\255\346\224\276", Q_NULLPTR));
        pushButton_continue->setText(QApplication::translate("MainWindow", "\347\273\247\347\273\255\346\222\255\346\224\276", Q_NULLPTR));
        pushButton_stop->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\347\272\277\347\250\213", Q_NULLPTR));
        pushButton_startRecord->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\345\275\225\345\210\266", Q_NULLPTR));
        pushButton_stopRecord->setText(QApplication::translate("MainWindow", "\344\270\255\346\226\255\345\275\225\345\210\266", Q_NULLPTR));
        label_frameInfo->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\345\270\247\346\225\260: 0 / \346\200\273\345\270\247\346\225\260: 0", Q_NULLPTR));
        label_video->setStyleSheet(QApplication::translate("MainWindow", "border: 1px solid gray;", Q_NULLPTR));
        label_video->setText(QApplication::translate("MainWindow", "\350\247\206\351\242\221\346\222\255\346\224\276\345\214\272\345\237\237", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
