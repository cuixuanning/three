#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_videoThread(nullptr)
    , m_videoWriter(nullptr)
    , m_isRecording(false)
    , m_isSliderPressed(false)
{
    ui->setupUi(this);
    
    // 设置视频路径和保存目录
    m_videoPath = "./src/playvideo.mp4";
    m_saveDir = "./save/";
    
    // 初始化
    initUI();
    createDirectories();
    initVideoThread();
}

MainWindow::~MainWindow()
{
    // 停止录制
    if (m_isRecording) {
        on_pushButton_stopRecord_clicked();
    }
    
    // 停止视频线程
    if (m_videoThread) {
        m_videoThread->stopPlay();
        m_videoThread->wait(3000);// 确保线程结束
        delete m_videoThread;
    }
    
    delete ui;
}

void MainWindow::initUI()
{
    // 设置窗口标题
    this->setWindowTitle("Qt OpenCV 视频播放与录制");
    
    // 初始化进度条
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(0);
    ui->horizontalSlider->setValue(0);
    
    // 初始化帧数显示
    updateFrameDisplay(0, 0);
    
    // 初始化按钮状态
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_continue->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    ui->pushButton_stopRecord->setEnabled(false);
}

void MainWindow::initVideoThread()
{
    m_videoThread = new VideoPlayThread(this);
    
    // 连接信号和槽
    connect(m_videoThread, &VideoPlayThread::frameReady, this, &MainWindow::onFrameReady);
    connect(m_videoThread, &VideoPlayThread::frameChanged, this, &MainWindow::onFrameChanged);
    connect(m_videoThread, &VideoPlayThread::playFinished, this, &MainWindow::onPlayFinished);
    
    // 设置视频路径
    m_videoThread->setVideoPath(m_videoPath);
    
    // 启动线程（但不开始播放，等待startPlay信号）
    if (!m_videoThread->isRunning()) {
        m_videoThread->start();
        // 等待线程初始化完成
        QThread::msleep(200);
    }
}

void MainWindow::createDirectories()
{
    // 创建src目录（如果不存在）
    QDir dir;
    if (!dir.exists("./src")) {
        dir.mkpath("./src");
        qDebug() << "创建目录: ./src";
    }
    
    // 创建save目录（如果不存在）
    if (!dir.exists(m_saveDir)) {
        dir.mkpath(m_saveDir);
        qDebug() << "创建目录:" << m_saveDir;
    }
}

QString MainWindow::generateSaveFileName()
{
    QDateTime now = QDateTime::currentDateTime();
    QString fileName = QString("record_%1.avi").arg(now.toString("yyyyMMdd_hhmmss"));
    return m_saveDir + fileName;
}

void MainWindow::updateFrameDisplay(int currentFrame, int totalFrames)
{
    ui->label_frameInfo->setText(QString("当前帧数: %1 / 总帧数: %2")
                                  .arg(currentFrame)
                                  .arg(totalFrames));
}

// 播放控制槽函数
void MainWindow::on_pushButton_start_clicked()
{
    if (!m_videoThread) {
        QMessageBox::warning(this, "错误", "视频线程未初始化");
        return;
    }
    
    // 检查视频文件是否存在
    QFileInfo fileInfo(m_videoPath);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "错误", QString("视频文件不存在:\n%1").arg(m_videoPath));
        return;
    }
    
    // 如果线程未运行，先启动
    if (!m_videoThread->isRunning()) {
        m_videoThread->start();
        // 等待线程初始化完成（视频文件打开）
        QThread::msleep(500);
    }
    
    // 开始播放（如果线程已运行，直接调用startPlay即可）
    m_videoThread->startPlay();
    
    // 更新按钮状态
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_continue->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
    
    // 更新进度条最大值（如果视频已加载）
    if (m_videoThread->getTotalFrames() > 0) {
        ui->horizontalSlider->setMaximum(m_videoThread->getTotalFrames() - 1);
    }
}

void MainWindow::on_pushButton_pause_clicked()
{
    if (m_videoThread) {
        m_videoThread->pausePlay();
        ui->pushButton_pause->setEnabled(false);
        ui->pushButton_continue->setEnabled(true);
    }
}

void MainWindow::on_pushButton_continue_clicked()
{
    if (m_videoThread) {
        m_videoThread->continuePlay();
        ui->pushButton_pause->setEnabled(true);
        ui->pushButton_continue->setEnabled(false);
    }
}

void MainWindow::on_pushButton_stop_clicked()
{
    if (m_videoThread) {
        m_videoThread->stopPlay();
        
        // 更新按钮状态
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_pause->setEnabled(false);
        ui->pushButton_continue->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        
        // 重置进度条
        ui->horizontalSlider->setValue(0);
        updateFrameDisplay(0, m_videoThread->getTotalFrames());
        
        // 清空视频显示
        ui->label_video->clear();
        ui->label_video->setText("视频播放区域");
    }
}

// 录制控制槽函数
void MainWindow::on_pushButton_startRecord_clicked()
{
    if (m_isRecording) {
        QMessageBox::information(this, "提示", "正在录制中，请先停止当前录制");
        return;
    }
    
    // 生成保存文件名
    m_currentRecordFile = generateSaveFileName();
    
    // 获取当前视频的尺寸和帧率
    if (m_videoThread && m_videoThread->getTotalFrames() > 0) {
        // 从视频线程获取信息
        m_recordFPS = m_videoThread->getFPS();
        if (m_recordFPS <= 0) {
            m_recordFPS = 25.0;
        }
        
        // 从label获取显示尺寸
        QSize labelSize = ui->label_video->size();
        m_recordSize = Size(labelSize.width(), labelSize.height());
    } else {
        // 默认值
        m_recordSize = Size(640, 480);
        m_recordFPS = 25.0;
    }
    
    // 创建VideoWriter
    int fourcc = VideoWriter::fourcc('X', 'V', 'I', 'D'); // XVID编码
    m_videoWriter = new VideoWriter();
    
    string filePath = m_currentRecordFile.toLocal8Bit().constData();
    if (!m_videoWriter->open(filePath, fourcc, m_recordFPS, m_recordSize, true)) {
        QMessageBox::critical(this, "错误", "无法创建视频文件，请检查保存目录权限");
        delete m_videoWriter;
        m_videoWriter = nullptr;
        return;
    }
    
    m_isRecording = true;
    ui->pushButton_startRecord->setEnabled(false);
    ui->pushButton_stopRecord->setEnabled(true);
    
    QMessageBox::information(this, "提示", QString("开始录制\n保存路径: %1").arg(m_currentRecordFile));
}

void MainWindow::on_pushButton_stopRecord_clicked()
{
    if (!m_isRecording || !m_videoWriter) {
        return;
    }
    
    // 停止录制
    m_videoWriter->release();
    delete m_videoWriter;
    m_videoWriter = nullptr;
    
    m_isRecording = false;
    ui->pushButton_startRecord->setEnabled(true);
    ui->pushButton_stopRecord->setEnabled(false);
    
    QMessageBox::information(this, "提示", QString("录制已停止\n文件已保存: %1").arg(m_currentRecordFile));
    m_currentRecordFile.clear();
}

// 进度条槽函数
void MainWindow::on_horizontalSlider_sliderPressed()
{
    m_isSliderPressed = true;
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    m_isSliderPressed = false;
    
    if (m_videoThread && m_videoThread->isRunning()) {
        int framePos = ui->horizontalSlider->value();
        // 跳转到指定帧
        m_videoThread->setFramePosition(framePos);
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    // 当用户拖动进度条时，不更新显示（避免冲突）
    if (m_isSliderPressed) {
        return;
    }
}

// VideoPlayThread信号槽
void MainWindow::onFrameReady(const QImage &image)
{
    if (image.isNull()) {
        return;
    }
    
    // 缩放图像以适应label大小
    QSize labelSize = ui->label_video->size();
    QPixmap pixmap = QPixmap::fromImage(image);
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    ui->label_video->setPixmap(scaledPixmap);
    
    // 如果正在录制，保存当前帧
    if (m_isRecording && m_videoWriter && m_videoWriter->isOpened()) {
        // 将QImage转换为Mat
        QImage recordImage = image.scaled(m_recordSize.width, m_recordSize.height, 
                                          Qt::KeepAspectRatio, Qt::SmoothTransformation);
        
        // 创建与录制尺寸相同的Mat
        Mat mat(m_recordSize.height, m_recordSize.width, CV_8UC3);
        
        // 如果图像尺寸不匹配，需要填充或裁剪
        if (recordImage.width() == m_recordSize.width && 
            recordImage.height() == m_recordSize.height) {
            // 直接转换
            Mat tempMat(recordImage.height(), recordImage.width(), CV_8UC3,
                           (void*)recordImage.constBits(), recordImage.bytesPerLine());
            cvtColor(tempMat, mat, COLOR_RGB2BGR);
        } else {
            // 需要填充或裁剪到目标尺寸
            Mat tempMat(recordImage.height(), recordImage.width(), CV_8UC3,
                           (void*)recordImage.constBits(), recordImage.bytesPerLine());
            Mat rgbMat;
            cvtColor(tempMat, rgbMat, COLOR_RGB2BGR);
            
            // 创建黑色背景
            mat = Mat::zeros(m_recordSize.height, m_recordSize.width, CV_8UC3);
            
            // 计算居中位置
            int x = (m_recordSize.width - rgbMat.cols) / 2;
            int y = (m_recordSize.height - rgbMat.rows) / 2;
            
            // 复制图像到中心
            rgbMat.copyTo(mat(Rect(x, y, rgbMat.cols, rgbMat.rows)));
        }
        
        *m_videoWriter << mat;
    }
}

void MainWindow::onFrameChanged(int currentFrame, int totalFrames)
{
    updateFrameDisplay(currentFrame, totalFrames);
    
    // 更新进度条（如果用户没有在拖动）
    if (!m_isSliderPressed && totalFrames > 0) {
        ui->horizontalSlider->setMaximum(totalFrames - 1);
        ui->horizontalSlider->setValue(currentFrame);
    }
}

void MainWindow::onPlayFinished()
{
    // 播放完成
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_continue->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    
    QMessageBox::information(this, "提示", "视频播放完成");
}
