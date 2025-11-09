#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QThread>
#include "videoplaythread.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 播放控制槽函数
    void on_pushButton_start_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_continue_clicked();
    void on_pushButton_stop_clicked();
    
    // 录制控制槽函数
    void on_pushButton_startRecord_clicked();
    void on_pushButton_stopRecord_clicked();
    
    // 进度条槽函数
    void on_horizontalSlider_sliderPressed();// 开始拖动
    void on_horizontalSlider_sliderReleased();// 结束拖动
    // 滑块值发生改变
    void on_horizontalSlider_valueChanged(int value);
    
    // VideoPlayThread信号槽
    void onFrameReady(const QImage &image);
    void onFrameChanged(int currentFrame, int totalFrames);
    void onPlayFinished();

private:
    Ui::MainWindow *ui;
    VideoPlayThread *m_videoThread;
    
    // 视频路径和保存路径
    QString m_videoPath;
    QString m_saveDir;
    
    // 录制相关
    VideoWriter *m_videoWriter; // OpenCV视频写入对象
    bool m_isRecording;         // 是否正在录制
    QString m_currentRecordFile;// 当前录制文件名
    Size m_recordSize;          // 录制视频尺寸
    double m_recordFPS;         // 录制帧率
    
    // 进度条控制
    bool m_isSliderPressed;// 用户是否正在拖动
    
    // 初始化方法
    void initUI();
    void initVideoThread();
    void createDirectories();// 检查和创建视频播放和保存目录
    QString generateSaveFileName();// 录制文件名
    // 当前/总 帧率标签更新
    void updateFrameDisplay(int currentFrame, int totalFrames);
};

#endif // MAINWINDOW_H
