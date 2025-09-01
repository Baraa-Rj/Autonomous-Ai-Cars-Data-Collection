#include "gui/DisplayManager.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>
#include <QtCore/QStandardPaths>
#include <QMetaObject>
#include <opencv2/imgproc.hpp>
#include <memory>

DisplayManager::DisplayManager(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , dataManager(std::make_unique<DataManager>(this))
    , isPlaying(false)
    , playbackSpeed(1.0)
    , userDraggingSlider(false)
{
    setupUI();
    
    connect(dataManager.get(), &DataManager::dataLoaded, this, &DisplayManager::onDataLoaded);
    connect(dataManager.get(), &DataManager::dataLoadingProgress, this, &DisplayManager::onDataLoadingProgress);
    connect(dataManager.get(), &DataManager::dataLoadingError, this, &DisplayManager::onDataLoadingError);
    
    setWindowTitle("Car Status Visualization");
    setMinimumSize(1000, 600);
    resize(1200, 700);
}

DisplayManager::~DisplayManager() {
}

void DisplayManager::setupUI() {
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainVerticalLayout = new QVBoxLayout(centralWidget);
    
    QHBoxLayout* contentLayout = new QHBoxLayout;
    
    setupSensorPanel();
    setupImagePanel();
    setupMapPanel();
    setupControlPanel();
    
    contentLayout->addWidget(sensorPanel, 1);
    contentLayout->addWidget(imagePanel, 2);
    contentLayout->addWidget(mapPanel, 1);
    
    mainVerticalLayout->addLayout(contentLayout, 1);
    mainVerticalLayout->addWidget(controlPanel, 0); 
    
    mainVerticalLayout->setSpacing(5);
    contentLayout->setSpacing(10);
}

void DisplayManager::setupSensorPanel() {
    sensorPanel = new QGroupBox("Sensor Data");
    sensorLayout = new QVBoxLayout(sensorPanel);
    
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
    
    sensorLayout->addWidget(gpsLabel);
    sensorLayout->addWidget(imuLabel);
    sensorLayout->addWidget(speedLabel);
    sensorLayout->addWidget(brakeLabel);
    sensorLayout->addWidget(throttleLabel);
    sensorLayout->addWidget(steeringLabel);
    sensorLayout->addStretch();
}

void DisplayManager::setupImagePanel() {
    imagePanel = new QGroupBox("Camera Feeds");
    imageLayout = new QGridLayout(imagePanel);
    
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
    
    imageLayout->addWidget(frontImageLabel, 0, 0);
    imageLayout->addWidget(rightImageLabel, 0, 1);
    imageLayout->addWidget(leftImageLabel, 1, 0);
    imageLayout->addWidget(backImageLabel, 1, 1);
}

void DisplayManager::setupMapPanel() {
    mapPanel = new QGroupBox("GPS Map");
    mapLayout = new QVBoxLayout(mapPanel);
    
    gpsMapWidget = new GPSMapWidget(this);
    
    mapLayout->addWidget(gpsMapWidget);
}

void DisplayManager::setupControlPanel() {
    controlPanel = new QGroupBox("Controls");
    controlPanel->setMaximumHeight(80);
    controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(5, 5, 5, 5);
    
    loadButton = new QPushButton("Load Data");
    loadButton->setMaximumWidth(100);
    connect(loadButton, &QPushButton::clicked, this, &DisplayManager::loadData);
    
    
    playPauseButton = new QPushButton("Play");
    playPauseButton->setMaximumWidth(80);
    playPauseButton->setEnabled(false);
    connect(playPauseButton, &QPushButton::clicked, this, &DisplayManager::playPause);
    
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setEnabled(false);
    connect(timeSlider, &QSlider::valueChanged, this, &DisplayManager::onTimeSliderChanged);
    connect(timeSlider, &QSlider::sliderPressed, this, &DisplayManager::onTimeSliderPressed);
    connect(timeSlider, &QSlider::sliderReleased, this, &DisplayManager::onTimeSliderReleased);
    
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

void DisplayManager::loadData() {
    QString dataDir = QFileDialog::getExistingDirectory(this, 
                                                       "Select Data Directory", 
                                                       QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    
    if (dataDir.isEmpty()) return;
    
    loadButton->setEnabled(false);
    progressBar->setVisible(true);
    progressBar->setValue(0);
    
    dataManager->loadAllSensorDataAsync(dataDir.toStdString());
}


void DisplayManager::onDataLoaded() {
    loadButton->setEnabled(true);
    playPauseButton->setEnabled(true);
    timeSlider->setEnabled(true);
    progressBar->setVisible(false);
    
    ClockManager& clockManager = dataManager->getClockManager();
    
    timeSlider->setMinimum(0);
    timeSlider->setMaximum(ClockManager::SLIDER_MAX);
    timeSlider->setValue(0);
    
    clockManager.setCurrentTimestamp(clockManager.getMinTimestamp());
    updateSensorDisplays(clockManager.getCurrentTimestamp());
    updateImageDisplays(clockManager.getCurrentTimestamp());
    
    QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(clockManager.getCurrentTimestamp()));
    QString durationTime = QString::fromStdString(clockManager.formatDuration());
    timeLabel->setText(elapsedTime + " / " + durationTime);
    
    QMessageBox::information(this, "Success", "Data loaded successfully!");
}

void DisplayManager::onDataLoadingProgress(int percentage) {
    progressBar->setValue(percentage);
}

void DisplayManager::onDataLoadingError(const QString& error) {
    loadButton->setEnabled(true);
    progressBar->setVisible(false);
    QMessageBox::critical(this, "Error", "Failed to load data:\n" + error);
}

void DisplayManager::playPause() {
    ClockManager& clockManager = dataManager->getClockManager();
    
    if (isPlaying) {
        clockManager.stopTiming();
        playPauseButton->setText("Play");
        isPlaying = false;
    } else {
        clockManager.setPlaybackSpeed(playbackSpeed);
        clockManager.startTiming([this]() {
            // This callback will be executed in the timing thread
            // We need to use QMetaObject::invokeMethod to call updateDisplay on the main thread
            QMetaObject::invokeMethod(this, "updateDisplay", Qt::QueuedConnection);
        }, 33); // ~30 FPS
        playPauseButton->setText("Pause");
        isPlaying = true;
    }
}

void DisplayManager::onTimeSliderChanged(int value) {
    // Only respond to slider changes when user is dragging or when paused
    if (userDraggingSlider || !isPlaying) { 
        ClockManager& clockManager = dataManager->getClockManager();
        
        clockManager.setProgressFromSlider(value);
        double currentTime = clockManager.getCurrentTimestamp();
        
        updateSensorDisplays(currentTime);
        updateImageDisplays(currentTime);
        
        QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
        QString durationTime = QString::fromStdString(clockManager.formatDuration());
        timeLabel->setText(elapsedTime + " / " + durationTime);
    }
}

void DisplayManager::onTimeSliderPressed() {
    userDraggingSlider = true;
}

void DisplayManager::onTimeSliderReleased() {
    userDraggingSlider = false;
    
    // Update displays with final slider position
    if (timeSlider->isEnabled()) {
        ClockManager& clockManager = dataManager->getClockManager();
        clockManager.setProgressFromSlider(timeSlider->value());
        double currentTime = clockManager.getCurrentTimestamp();
        
        updateSensorDisplays(currentTime);
        updateImageDisplays(currentTime);
        
        QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
        QString durationTime = QString::fromStdString(clockManager.formatDuration());
        timeLabel->setText(elapsedTime + " / " + durationTime);
    }
}

void DisplayManager::updateDisplay() {
    if (!isPlaying) return;
    
    ClockManager& clockManager = dataManager->getClockManager();
    
    // Timing advancement is now handled in ClockManager's timing thread
    double currentTime = clockManager.getCurrentTimestamp();
    
    // Check if timing has stopped due to reaching the end
    if (!clockManager.isTimingActive()) {
        isPlaying = false;
        playPauseButton->setText("Play");
    }
    
    updateSensorDisplays(currentTime);
    updateImageDisplays(currentTime);
    
    // Only update slider if user is not dragging it
    if (!userDraggingSlider) {
        int sliderValue = clockManager.getSliderFromProgress();
        timeSlider->setValue(sliderValue);
    }
    
    QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
    QString durationTime = QString::fromStdString(clockManager.formatDuration());
    timeLabel->setText(elapsedTime + " / " + durationTime);
}

void DisplayManager::updateSensorDisplays(double timestamp) {
    // First update the sensor data to the target timestamp
    dataManager->updateSensorData(timestamp);
    
    auto gps = dataManager->getCurrentGPS();
    if (gps) {
        gpsLabel->setText(QString::fromStdString(gps->toString()));
        // Update GPS map with current position - add null check for safety
        if (gpsMapWidget) {
            gpsMapWidget->updateGPSPosition(gps);
        }
    }
    
    auto imu = dataManager->getCurrentIMU();
    if (imu) {
        imuLabel->setText(QString::fromStdString(imu->toString()));
    }
    
    auto speed = dataManager->getCurrentSpeed();
    if (speed) {
        speedLabel->setText(QString::fromStdString(speed->toString()));
    }
    
    auto brake = dataManager->getCurrentBrake();
    if (brake) {
        brakeLabel->setText(QString::fromStdString(brake->toString()));
    }
    
    auto throttle = dataManager->getCurrentThrottle();
    if (throttle) {
        throttleLabel->setText(QString::fromStdString(throttle->toString()));
    }
    
    auto steering = dataManager->getCurrentSteering();
    if (steering) {
        steeringLabel->setText(QString::fromStdString(steering->toString()));
    }
}

void DisplayManager::updateImageDisplays(double timestamp) {
    // Simple approach: load and display images directly
    displayImage(dataManager->getCurrentFrontImage(), frontImageLabel);
    displayImage(dataManager->getCurrentBackImage(), backImageLabel);
    displayImage(dataManager->getCurrentLeftImage(), leftImageLabel);
    displayImage(dataManager->getCurrentRightImage(), rightImageLabel);
}

// Simple image display without complex caching
void DisplayManager::displayImage(ImageData* imageData, QLabel* label) {
    if (!imageData || !label) return;
    
    // Load image if needed
    if (!imageData->isLoaded()) {
        imageData->loadImageAsync(); // Actually synchronous
    }
    
    // Display if loaded
    if (imageData->isLoaded() && !imageData->isEmpty()) {
        cv::Mat image = imageData->getImage();
        QPixmap pixmap = matToQPixmap(image);
        QPixmap scaled = pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(scaled);
    }
}

QPixmap DisplayManager::matToQPixmap(const cv::Mat& mat) {
    QImage qimg;
    
    if (mat.channels() == 3) {
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
        qimg = QImage(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888).copy();
    } else if (mat.channels() == 1) {
        qimg = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    } else {
        return QPixmap(); 
    }
    
    return QPixmap::fromImage(qimg);
}