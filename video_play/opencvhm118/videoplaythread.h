#ifndef VIDEOPLAYTHREAD_H
#define VIDEOPLAYTHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>

class VideoPlayThread : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayThread(QObject *parent = nullptr);
    ~VideoPlayThread();

    // 设置视频路径
    void setVideoPath(const QString &path);

    // 控制函数
    void startPlay();
    void pausePlay();
    void continuePlay();
    void stopPlay();
    
    // 跳转到指定帧
    void setFramePosition(int frameNumber);

    // 获取视频信息
    int getTotalFrames() const { return m_totalFrames; }
    int getCurrentFrame() const { return m_currentFrame; }
    double getFPS() const { return m_fps; }

signals:
    // 发送视频帧
    void frameReady(const QImage &image);
    // 发送帧数信息
    void frameChanged(int currentFrame, int totalFrames);
    // 播放完成
    void playFinished();

protected:
    void run() override;

private:
    QString m_videoPath;
    cv::VideoCapture *m_capture;
    
    // 线程控制
    bool m_isRunning;
    bool m_isPaused;
    bool m_shouldStop;
    bool m_needSeek;
    int m_seekFrame;
    
    // 视频信息
    int m_totalFrames;
    int m_currentFrame;
    double m_fps;
    
    // 线程同步
    QMutex m_mutex;
    QWaitCondition m_pauseCondition;
};

#endif // VIDEOPLAYTHREAD_H

