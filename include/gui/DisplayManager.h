#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QGroupBox>
#include <QtCore/QTimer>
#include <opencv2/opencv.hpp>
#include <memory>
#include "management/DataManager.h"
#include "gui/GPSMapWidget.h"

class DisplayManager : public QMainWindow {
    Q_OBJECT

public:
    DisplayManager(QWidget *parent = nullptr);
    ~DisplayManager();

private slots:
    void loadData();
    void onDataLoaded();
    void onDataLoadingProgress(int percentage);
    void onDataLoadingError(const QString& error);
    void playPause();
    void onTimeSliderChanged(int value);
    void updateDisplay();

private:
    void setupUI();
    void setupSensorPanel();
    void setupImagePanel();
    void setupMapPanel();
    void setupControlPanel();
    
    void updateSensorDisplays(double timestamp);
    void updateImageDisplays(double timestamp);
    QPixmap matToQPixmap(const cv::Mat& mat);
    
    // Cache for scaled pixmaps
    QPixmap frontPixmapCache;
    QPixmap backPixmapCache;
    QPixmap leftPixmapCache;
    QPixmap rightPixmapCache;
    ImageData* lastFrontImage;
    ImageData* lastBackImage;
    ImageData* lastLeftImage;
    ImageData* lastRightImage;
    
    // UI Components
    QWidget* centralWidget;
    
    // Left panel - Sensor data
    QGroupBox* sensorPanel;
    QVBoxLayout* sensorLayout;
    QLabel* gpsLabel;
    QLabel* imuLabel;
    QLabel* speedLabel;
    QLabel* brakeLabel;
    QLabel* throttleLabel;
    QLabel* steeringLabel;
    
    // Right panel - Camera feeds
    QGroupBox* imagePanel;
    QGridLayout* imageLayout;
    QLabel* frontImageLabel;
    QLabel* backImageLabel;
    QLabel* leftImageLabel;
    QLabel* rightImageLabel;
    
    // GPS Map panel
    QGroupBox* mapPanel;
    QVBoxLayout* mapLayout;
    GPSMapWidget* gpsMapWidget;
    
    // Control panel
    QGroupBox* controlPanel;
    QHBoxLayout* controlLayout;
    QPushButton* loadButton;
    QPushButton* playPauseButton;
    QSlider* timeSlider;
    QLabel* timeLabel;
    QProgressBar* progressBar;
    
    // Core components
    std::unique_ptr<DataManager> dataManager;
    std::unique_ptr<QTimer> playbackTimer;
    
    // Playback state
    bool isPlaying;
    double playbackSpeed; // seconds per second (1.0 = real time)
};

// MOC file will be generated automatically