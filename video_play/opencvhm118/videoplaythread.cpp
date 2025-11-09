#include "videoplaythread.h"
#include <QDebug>
#include <QDir>

VideoPlayThread::VideoPlayThread(QObject *parent)
    : QThread(parent)
    , m_capture(nullptr)
    , m_isRunning(false)
    , m_isPaused(false)
    , m_shouldStop(false)
    , m_needSeek(false)
    , m_seekFrame(0)
    , m_totalFrames(0)
    , m_currentFrame(0)
    , m_fps(25.0)
{
}

VideoPlayThread::~VideoPlayThread()
{
    // 真正停止线程
    {
        QMutexLocker locker(&m_mutex);
        m_shouldStop = true;
        m_isRunning = false;
        m_pauseCondition.wakeAll();
    }
    
    // 等待线程结束
    if (isRunning()) {
        wait(3000);
    }
    
    if (m_capture) {
        m_capture->release();
        delete m_capture;
        m_capture = nullptr;
    }
}

void VideoPlayThread::setVideoPath(const QString &path)
{
    QMutexLocker locker(&m_mutex);
    m_videoPath = path;
}

void VideoPlayThread::startPlay()
{
    QMutexLocker locker(&m_mutex);
    // 如果视频已打开，重置到开头
    if (m_capture && m_capture->isOpened()) {
        m_capture->set(cv::CAP_PROP_POS_FRAMES, 0);
    }
    m_currentFrame = 0;
    m_isRunning = true;
    m_isPaused = false;
    m_shouldStop = false;
    m_pauseCondition.wakeAll();
}

void VideoPlayThread::pausePlay()
{
    QMutexLocker locker(&m_mutex);
    m_isPaused = true;
}

void VideoPlayThread::continuePlay()
{
    QMutexLocker locker(&m_mutex);
    m_isPaused = false;
    m_pauseCondition.wakeAll();
}

void VideoPlayThread::stopPlay()
{
    QMutexLocker locker(&m_mutex);
    m_shouldStop = false;  // 不真正停止线程，只是停止播放
    m_isRunning = false;
    m_isPaused = false;
    m_currentFrame = 0;
    // 重置视频到开头
    if (m_capture && m_capture->isOpened()) {
        m_capture->set(cv::CAP_PROP_POS_FRAMES, 0);
    }
    m_pauseCondition.wakeAll();
}

void VideoPlayThread::setFramePosition(int frameNumber)
{
    QMutexLocker locker(&m_mutex);
    if (frameNumber >= 0 && frameNumber < m_totalFrames && m_capture && m_capture->isOpened()) {
        m_seekFrame = frameNumber;
        m_needSeek = true;
        m_currentFrame = frameNumber;
        m_pauseCondition.wakeAll();
    }
}

void VideoPlayThread::run()
{
    // 打开视频文件
    if (!m_capture) {
        m_capture = new cv::VideoCapture();
    }
    
    if (m_capture->isOpened()) {
        m_capture->release();
    }
    
    QString videoPath;
    {
        QMutexLocker locker(&m_mutex);
        videoPath = m_videoPath;
    }
    
    if (videoPath.isEmpty()) {
        qDebug() << "视频路径为空";
        return;
    }
    
    // 转换路径格式（Windows路径处理）
    std::string path = videoPath.toLocal8Bit().constData();
    if (!m_capture->open(path)) {
        qDebug() << "无法打开视频文件:" << videoPath;
        return;
    }
    
    // 获取视频信息
    {
        QMutexLocker locker(&m_mutex);
        m_totalFrames = static_cast<int>(m_capture->get(cv::CAP_PROP_FRAME_COUNT));
        m_fps = m_capture->get(cv::CAP_PROP_FPS);
        if (m_fps <= 0) {
            m_fps = 25.0; // 默认帧率
        }
    }
    
    qDebug() << "视频信息 - 总帧数:" << m_totalFrames << "FPS:" << m_fps;
    
    cv::Mat frame;
    int frameDelay = static_cast<int>(1000.0 / m_fps); // 毫秒
    
    // 等待开始播放信号
    {
        QMutexLocker locker(&m_mutex);
        while (!m_isRunning && !m_shouldStop) {
            m_pauseCondition.wait(&m_mutex, 100);
        }
    }
    
    while (!m_shouldStop && m_capture->isOpened()) {
        // 检查是否需要跳转
        {
            QMutexLocker locker(&m_mutex);
            if (m_needSeek) {
                m_capture->set(cv::CAP_PROP_POS_FRAMES, m_seekFrame);
                m_currentFrame = m_seekFrame;
                m_needSeek = false;
            }
        }
        
        // 检查暂停状态
        {
            QMutexLocker locker(&m_mutex);
            while (m_isPaused && m_isRunning && !m_shouldStop) {
                m_pauseCondition.wait(&m_mutex);
            }
            
            if (m_shouldStop) {
                break;
            }
            
            if (!m_isRunning) {
                // 等待开始播放
                m_pauseCondition.wait(&m_mutex, 100);
                continue;
            }
        }
        
        // 读取帧
        if (!m_capture->read(frame) || frame.empty()) {
            // 视频播放完成
            {
                QMutexLocker locker(&m_mutex);
                m_isRunning = false;
            }
            emit playFinished();
            // 等待下一次播放，而不是退出
            {
                QMutexLocker locker(&m_mutex);
                while (!m_isRunning && !m_shouldStop) {
                    m_pauseCondition.wait(&m_mutex, 100);
                }
            }
            continue;
        }
        
        // 转换Mat到QImage
        QImage image;
        if (frame.channels() == 3) {
            cv::Mat rgbFrame;
            cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
            image = QImage(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888).copy();
        } else if (frame.channels() == 1) {
            image = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Grayscale8).copy();
        }
        
        if (!image.isNull()) {
            // 发送帧
            emit frameReady(image);
            {
                QMutexLocker locker(&m_mutex);
                emit frameChanged(m_currentFrame, m_totalFrames);
            }
        }
        
        {
            QMutexLocker locker(&m_mutex);
            m_currentFrame++;
        }
        
        // 控制播放速度
        msleep(frameDelay);
    }
    
    {
        QMutexLocker locker(&m_mutex);
        m_isRunning = false;
    }
    qDebug() << "视频播放线程结束";
}

