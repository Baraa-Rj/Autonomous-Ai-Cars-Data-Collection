#include "gui/MainWindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>
#include <QtCore/QStandardPaths>
#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , dataManager(new DataManager(this))
    , playbackTimer(new QTimer(this))
    , isPlaying(false)
    , playbackSpeed(1.0)
{
    setupUI();
    
    // Connect signals
    connect(dataManager, &DataManager::dataLoaded, this, &MainWindow::onDataLoaded);
    connect(dataManager, &DataManager::dataLoadingProgress, this, &MainWindow::onDataLoadingProgress);
    connect(dataManager, &DataManager::dataLoadingError, this, &MainWindow::onDataLoadingError);
    
    connect(playbackTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    playbackTimer->setInterval(50); // 20 FPS update rate
    
    // Set window properties
    setWindowTitle("Car Status Visualization");
    setMinimumSize(1000, 600);
    resize(1200, 700);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    
    // Create main vertical layout
    QVBoxLayout* mainVerticalLayout = new QVBoxLayout(centralWidget);
    
    // Create horizontal layout for main content
    QHBoxLayout* contentLayout = new QHBoxLayout;
    
    setupSensorPanel();
    setupImagePanel();
    setupControlPanel();
    
    // Add sensor and image panels to content layout
    contentLayout->addWidget(sensorPanel, 1);
    contentLayout->addWidget(imagePanel, 2);
    
    // Add content layout and control panel to main vertical layout
    mainVerticalLayout->addLayout(contentLayout, 1);
    mainVerticalLayout->addWidget(controlPanel, 0); // 0 means fixed height
    
    // Set layout spacing
    mainVerticalLayout->setSpacing(5);
    contentLayout->setSpacing(10);
}

void MainWindow::setupSensorPanel() {
    sensorPanel = new QGroupBox("Sensor Data");
    sensorLayout = new QVBoxLayout(sensorPanel);
    
    // Create labels for each sensor
    gpsLabel = new QLabel("GPS: No data");
    gpsLabel->setWordWrap(true);
    gpsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    gpsLabel->setMaximumWidth(250);
    gpsLabel->setMinimumHeight(70);
    gpsLabel->setStyleSheet("QLabel { font-size: 11px; font-family: monospace; margin: 5px; padding: 5px; background-color: #f0f0f0; border-radius: 3px; }");
    
    imuLabel = new QLabel("IMU: No data");
    imuLabel->setWordWrap(true);
    imuLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    imuLabel->setMaximumWidth(250);
    imuLabel->setMinimumHeight(80);
    imuLabel->setStyleSheet("QLabel { font-size: 11px; font-family: monospace; margin: 5px; padding: 5px; background-color: #f0f0f0; border-radius: 3px; }");
    
    speedLabel = new QLabel("Speed: No data");
    speedLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; margin: 5px; padding: 8px; background-color: #e8f4f8; border-radius: 3px; }");
    
    brakeLabel = new QLabel("Brake: No data");
    brakeLabel->setStyleSheet("QLabel { font-size: 12px; margin: 5px; padding: 5px; background-color: #ffe8e8; border-radius: 3px; }");
    
    throttleLabel = new QLabel("Throttle: No data");
    throttleLabel->setStyleSheet("QLabel { font-size: 12px; margin: 5px; padding: 5px; background-color: #e8f8e8; border-radius: 3px; }");
    
    steeringLabel = new QLabel("Steering: No data");
    steeringLabel->setStyleSheet("QLabel { font-size: 12px; margin: 5px; padding: 5px; background-color: #f8f8e8; border-radius: 3px; }");
    
    // Add labels to layout
    sensorLayout->addWidget(gpsLabel);
    sensorLayout->addWidget(imuLabel);
    sensorLayout->addWidget(speedLabel);
    sensorLayout->addWidget(brakeLabel);
    sensorLayout->addWidget(throttleLabel);
    sensorLayout->addWidget(steeringLabel);
    sensorLayout->addStretch(); // Add stretch to push content to top
}

void MainWindow::setupImagePanel() {
    imagePanel = new QGroupBox("Camera Feeds");
    imageLayout = new QGridLayout(imagePanel);
    
    // Create image labels for camera feeds
    frontImageLabel = new QLabel("Front Camera");
    frontImageLabel->setMinimumSize(200, 150);
    frontImageLabel->setMaximumSize(400, 300);
    frontImageLabel->setStyleSheet("QLabel { border: 2px solid #ccc; background-color: #f9f9f9; font-size: 12px; }");
    frontImageLabel->setAlignment(Qt::AlignCenter);
    frontImageLabel->setScaledContents(true);
    
    backImageLabel = new QLabel("Back Camera");
    backImageLabel->setMinimumSize(200, 150);
    backImageLabel->setMaximumSize(400, 300);
    backImageLabel->setStyleSheet("QLabel { border: 2px solid #ccc; background-color: #f9f9f9; font-size: 12px; }");
    backImageLabel->setAlignment(Qt::AlignCenter);
    backImageLabel->setScaledContents(true);
    
    leftImageLabel = new QLabel("Left Camera");
    leftImageLabel->setMinimumSize(200, 150);
    leftImageLabel->setMaximumSize(400, 300);
    leftImageLabel->setStyleSheet("QLabel { border: 2px solid #ccc; background-color: #f9f9f9; font-size: 12px; }");
    leftImageLabel->setAlignment(Qt::AlignCenter);
    leftImageLabel->setScaledContents(true);
    
    rightImageLabel = new QLabel("Right Camera");
    rightImageLabel->setMinimumSize(200, 150);
    rightImageLabel->setMaximumSize(400, 300);
    rightImageLabel->setStyleSheet("QLabel { border: 2px solid #ccc; background-color: #f9f9f9; font-size: 12px; }");
    rightImageLabel->setAlignment(Qt::AlignCenter);
    rightImageLabel->setScaledContents(true);
    
    // Arrange in 2x2 grid
    imageLayout->addWidget(frontImageLabel, 0, 0);
    imageLayout->addWidget(rightImageLabel, 0, 1);
    imageLayout->addWidget(leftImageLabel, 1, 0);
    imageLayout->addWidget(backImageLabel, 1, 1);
}

void MainWindow::setupControlPanel() {
    controlPanel = new QGroupBox("Controls");
    controlPanel->setMaximumHeight(80);
    controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(5, 5, 5, 5);
    
    loadButton = new QPushButton("Load Data");
    loadButton->setMaximumWidth(100);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadData);
    
    playPauseButton = new QPushButton("Play");
    playPauseButton->setMaximumWidth(80);
    playPauseButton->setEnabled(false);
    connect(playPauseButton, &QPushButton::clicked, this, &MainWindow::playPause);
    
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setEnabled(false);
    connect(timeSlider, &QSlider::valueChanged, this, &MainWindow::onTimeSliderChanged);
    
    timeLabel = new QLabel("00:00 / 00:00");
    timeLabel->setMinimumWidth(80);
    timeLabel->setStyleSheet("QLabel { font-family: monospace; }");
    
    progressBar = new QProgressBar();
    progressBar->setVisible(false);
    progressBar->setMaximumHeight(20);
    
    controlLayout->addWidget(loadButton);
    controlLayout->addWidget(playPauseButton);
    controlLayout->addWidget(timeSlider, 1);
    controlLayout->addWidget(timeLabel);
    controlLayout->addWidget(progressBar);
}

void MainWindow::loadData() {
    QString dataDir = QFileDialog::getExistingDirectory(this, 
                                                       "Select Data Directory", 
                                                       QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    
    if (dataDir.isEmpty()) return;
    
    loadButton->setEnabled(false);
    progressBar->setVisible(true);
    progressBar->setValue(0);
    
    // Load data in separate thread (simplified for now, loading in main thread)
    if (dataManager->loadAllSensorData(dataDir.toStdString())) {
        // Success handled in onDataLoaded slot
    } else {
        loadButton->setEnabled(true);
        progressBar->setVisible(false);
    }
}

void MainWindow::onDataLoaded() {
    loadButton->setEnabled(true);
    playPauseButton->setEnabled(true);
    timeSlider->setEnabled(true);
    progressBar->setVisible(false);
    
    // Setup time slider using ClockManager
    ClockManager& clockManager = dataManager->getClockManager();
    
    timeSlider->setMinimum(0);
    timeSlider->setMaximum(ClockManager::SLIDER_MAX);
    timeSlider->setValue(0);
    
    // Set initial position to start of data
    clockManager.setCurrentTimestamp(clockManager.getMinTimestamp());
    updateSensorDisplays(clockManager.getCurrentTimestamp());
    updateImageDisplays(clockManager.getCurrentTimestamp());
    
    // Update time label with formatted duration
    QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(clockManager.getCurrentTimestamp()));
    QString durationTime = QString::fromStdString(clockManager.formatDuration());
    timeLabel->setText(elapsedTime + " / " + durationTime);
    
    QMessageBox::information(this, "Success", "Data loaded successfully!");
}

void MainWindow::onDataLoadingProgress(int percentage) {
    progressBar->setValue(percentage);
}

void MainWindow::onDataLoadingError(const QString& error) {
    loadButton->setEnabled(true);
    progressBar->setVisible(false);
    QMessageBox::critical(this, "Error", "Failed to load data:\n" + error);
}

void MainWindow::playPause() {
    if (isPlaying) {
        playbackTimer->stop();
        playPauseButton->setText("Play");
        isPlaying = false;
    } else {
        playbackTimer->start();
        playPauseButton->setText("Pause");
        isPlaying = true;
    }
}

void MainWindow::onTimeSliderChanged(int value) {
    if (!isPlaying) { // Only update when not playing to avoid conflicts
        ClockManager& clockManager = dataManager->getClockManager();
        
        // Update timestamp from slider position
        clockManager.setProgressFromSlider(value);
        double currentTime = clockManager.getCurrentTimestamp();
        
        updateSensorDisplays(currentTime);
        updateImageDisplays(currentTime);
        
        // Update time label using ClockManager formatting
        QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
        QString durationTime = QString::fromStdString(clockManager.formatDuration());
        timeLabel->setText(elapsedTime + " / " + durationTime);
    }
}

void MainWindow::updateDisplay() {
    if (!isPlaying) return;
    
    ClockManager& clockManager = dataManager->getClockManager();
    
    // Advance time using ClockManager
    double deltaSeconds = playbackSpeed * (playbackTimer->interval() / 1000.0);
    clockManager.advanceTime(deltaSeconds);
    
    double currentTime = clockManager.getCurrentTimestamp();
    
    // Check if reached end of data
    if (currentTime >= clockManager.getMaxTimestamp()) {
        playPause(); // Auto-pause at end
    }
    
    // Update displays
    updateSensorDisplays(currentTime);
    updateImageDisplays(currentTime);
    
    // Update slider using ClockManager
    int sliderValue = clockManager.getSliderFromProgress();
    timeSlider->setValue(sliderValue);
    
    // Update time label using ClockManager formatting
    QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
    QString durationTime = QString::fromStdString(clockManager.formatDuration());
    timeLabel->setText(elapsedTime + " / " + durationTime);
}

void MainWindow::updateSensorDisplays(double timestamp) {
    // Update GPS
    auto gps = dataManager->getCurrentGPS(timestamp);
    if (gps) {
        gpsLabel->setText(QString::fromStdString(gps->toString()));
    }
    
    // Update IMU
    auto imu = dataManager->getCurrentIMU(timestamp);
    if (imu) {
        imuLabel->setText(QString::fromStdString(imu->toString()));
    }
    
    // Update Speed
    auto speed = dataManager->getCurrentSpeed(timestamp);
    if (speed) {
        speedLabel->setText(QString::fromStdString(speed->toString()));
    }
    
    // Update Brake
    auto brake = dataManager->getCurrentBrake(timestamp);
    if (brake) {
        brakeLabel->setText(QString::fromStdString(brake->toString()));
    }
    
    // Update Throttle
    auto throttle = dataManager->getCurrentThrottle(timestamp);
    if (throttle) {
        throttleLabel->setText(QString::fromStdString(throttle->toString()));
    }
    
    // Update Steering
    auto steering = dataManager->getCurrentSteering(timestamp);
    if (steering) {
        steeringLabel->setText(QString::fromStdString(steering->toString()));
    }
}

void MainWindow::updateImageDisplays(double timestamp) {
    // Get all images for current timestamp first
    auto frontImg = dataManager->getCurrentFrontImage(timestamp);
    auto backImg = dataManager->getCurrentBackImage(timestamp);
    auto leftImg = dataManager->getCurrentLeftImage(timestamp);
    auto rightImg = dataManager->getCurrentRightImage(timestamp);
    
    // Update all cameras simultaneously to prevent flickering
    if (frontImg) {
        if (!frontImg->loaded) frontImg->loadImage(); // Lazy load on demand
        if (!frontImg->image.empty()) {
            QPixmap pixmap = matToQPixmap(frontImg->image);
            frontImageLabel->setPixmap(pixmap.scaled(frontImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    
    if (backImg) {
        if (!backImg->loaded) backImg->loadImage(); // Lazy load on demand
        if (!backImg->image.empty()) {
            QPixmap pixmap = matToQPixmap(backImg->image);
            backImageLabel->setPixmap(pixmap.scaled(backImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    
    if (leftImg) {
        if (!leftImg->loaded) leftImg->loadImage(); // Lazy load on demand
        if (!leftImg->image.empty()) {
            QPixmap pixmap = matToQPixmap(leftImg->image);
            leftImageLabel->setPixmap(pixmap.scaled(leftImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    
    if (rightImg) {
        if (!rightImg->loaded) rightImg->loadImage(); // Lazy load on demand
        if (!rightImg->image.empty()) {
            QPixmap pixmap = matToQPixmap(rightImg->image);
            rightImageLabel->setPixmap(pixmap.scaled(rightImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

QPixmap MainWindow::matToQPixmap(const cv::Mat& mat) {
    QImage qimg;
    
    if (mat.channels() == 3) {
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
        // Create QImage with a copy of the data to avoid memory issues
        qimg = QImage(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888).copy();
    } else if (mat.channels() == 1) {
        // Create QImage with a copy of the data to avoid memory issues
        qimg = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    } else {
        return QPixmap(); // Unsupported format
    }
    
    return QPixmap::fromImage(qimg);
}