#include "gui/MainWindow.h"
#include "management/DataManager.h"
#include "controllers/PlaybackController.h"
#include "controllers/DisplayController.h"
#include "controllers/DataLoadingController.h"
#include "gui/GPSMapWidget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , userDraggingSlider(false)
{
    // Create data layer
    dataManager = std::make_unique<DataManager>(this);
    
    // Create controllers (SRP-compliant)
    playbackController = std::make_unique<PlaybackController>(&dataManager->getClockManager(), this);
    displayController = std::make_unique<DisplayController>(dataManager.get(), this);
    loadingController = std::make_unique<DataLoadingController>(dataManager.get(), this);
    
    setupUI();
    
    connect(loadingController.get(), &DataLoadingController::loadingFinished,
            this, &MainWindow::onDataLoaded);
    connect(loadingController.get(), &DataLoadingController::loadingProgress,
            this, &MainWindow::onLoadingProgress);
    connect(loadingController.get(), &DataLoadingController::loadingError,
            this, &MainWindow::onLoadingError);
    
    connect(playbackController.get(), &PlaybackController::positionChanged,
            this, &MainWindow::onPlaybackPositionChanged);
    
    setWindowTitle("Car Status Visualization");
    setMinimumSize(1000, 600);
    resize(1200, 700);
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
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
    
    // Configure display controller with UI labels
    displayController->setSensorLabels(gpsLabel, imuLabel, speedLabel, 
                                     brakeLabel, throttleLabel, steeringLabel);
    displayController->setImageLabels(frontImageLabel, backImageLabel, 
                                    leftImageLabel, rightImageLabel);
}

void MainWindow::setupSensorPanel() {
    sensorPanel = new QGroupBox("Sensor Data");
    QVBoxLayout* sensorLayout = new QVBoxLayout(sensorPanel);
    
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

void MainWindow::setupImagePanel() {
    imagePanel = new QGroupBox("Camera Feeds");
    QGridLayout* imageLayout = new QGridLayout(imagePanel);
    
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

void MainWindow::setupMapPanel() {
    mapPanel = new QGroupBox("GPS Map");
    QVBoxLayout* mapLayout = new QVBoxLayout(mapPanel);
    
    gpsMapWidget = new GPSMapWidget(this);
    mapLayout->addWidget(gpsMapWidget);
}

void MainWindow::setupControlPanel() {
    controlPanel = new QGroupBox("Controls");
    controlPanel->setMaximumHeight(80);
    QHBoxLayout* controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(5, 5, 5, 5);
    
    loadButton = new QPushButton("Load Data");
    loadButton->setMaximumWidth(100);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadData);
    
    playPauseButton = new QPushButton("Play");
    playPauseButton->setMaximumWidth(80);
    playPauseButton->setEnabled(false);
    connect(playPauseButton, &QPushButton::clicked, playbackController.get(), &PlaybackController::togglePlayPause);
    
    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setEnabled(false);
    connect(timeSlider, &QSlider::valueChanged, this, &MainWindow::onTimeSliderChanged);
    connect(timeSlider, &QSlider::sliderPressed, this, &MainWindow::onTimeSliderPressed);
    connect(timeSlider, &QSlider::sliderReleased, this, &MainWindow::onTimeSliderReleased);
    
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
    
    loadingController->loadData(dataDir);
}

void MainWindow::onDataLoaded() {
    loadButton->setEnabled(true);
    playPauseButton->setEnabled(true);
    timeSlider->setEnabled(true);
    progressBar->setVisible(false);
    
    auto& clockManager = dataManager->getClockManager();
    
    timeSlider->setMinimum(0);
    timeSlider->setMaximum(clockManager.SLIDER_MAX);
    timeSlider->setValue(0);
    
    clockManager.setCurrentTimestamp(clockManager.getMinTimestamp());
    displayController->updateDisplays(clockManager.getCurrentTimestamp());
    
    QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(clockManager.getCurrentTimestamp()));
    QString durationTime = QString::fromStdString(clockManager.formatDuration());
    timeLabel->setText(elapsedTime + " / " + durationTime);
    
    QMessageBox::information(this, "Success", "Data loaded successfully!");
}

void MainWindow::onLoadingProgress(int percentage) {
    progressBar->setValue(percentage);
}

void MainWindow::onLoadingError(const QString& error) {
    loadButton->setEnabled(true);
    progressBar->setVisible(false);
    QMessageBox::critical(this, "Error", "Failed to load data:\n" + error);
}

void MainWindow::onPlaybackPositionChanged(double timestamp) {
    displayController->updateDisplays(timestamp);
    
    // Update GPS map
    auto gps = dataManager->getCurrentGPS();
    if (gps && gpsMapWidget) {
        gpsMapWidget->updateGPSPosition(gps);
    }
    
    // Update slider if user is not dragging it
    if (!userDraggingSlider) {
        auto& clockManager = dataManager->getClockManager();
        int sliderValue = clockManager.getSliderFromProgress();
        timeSlider->setValue(sliderValue);
    }
    
    // Update time label
    auto& clockManager = dataManager->getClockManager();
    QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(timestamp));
    QString durationTime = QString::fromStdString(clockManager.formatDuration());
    timeLabel->setText(elapsedTime + " / " + durationTime);
    
    // Update play/pause button text
    if (playbackController->isPlaying()) {
        playPauseButton->setText("Pause");
    } else {
        playPauseButton->setText("Play");
    }
}

void MainWindow::onTimeSliderChanged(int value) {
    if (userDraggingSlider || !playbackController->isPlaying()) {
        auto& clockManager = dataManager->getClockManager();
        clockManager.setProgressFromSlider(value);
        double currentTime = clockManager.getCurrentTimestamp();
        
        displayController->updateDisplays(currentTime);
        
        QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
        QString durationTime = QString::fromStdString(clockManager.formatDuration());
        timeLabel->setText(elapsedTime + " / " + durationTime);
    }
}

void MainWindow::onTimeSliderPressed() {
    userDraggingSlider = true;
}

void MainWindow::onTimeSliderReleased() {
    userDraggingSlider = false;
    
    if (timeSlider->isEnabled()) {
        auto& clockManager = dataManager->getClockManager();
        clockManager.setProgressFromSlider(timeSlider->value());
        double currentTime = clockManager.getCurrentTimestamp();
        
        displayController->updateDisplays(currentTime);
        
        QString elapsedTime = QString::fromStdString(clockManager.formatElapsedTime(currentTime));
        QString durationTime = QString::fromStdString(clockManager.formatDuration());
        timeLabel->setText(elapsedTime + " / " + durationTime);
    }
}