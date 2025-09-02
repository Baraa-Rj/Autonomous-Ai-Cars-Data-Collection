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
#include <memory>

class DataManager;
class PlaybackController;
class DisplayController;
class DataLoadingController;
class GPSMapWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadData();
    void onDataLoaded();
    void onLoadingProgress(int percentage);
    void onLoadingError(const QString& error);
    void onPlaybackPositionChanged(double timestamp);
    void onTimeSliderChanged(int value);
    void onTimeSliderPressed();
    void onTimeSliderReleased();

private:
    void setupUI();
    void setupSensorPanel();
    void setupImagePanel();
    void setupMapPanel();
    void setupControlPanel();
    
    std::unique_ptr<DataManager> dataManager;
    std::unique_ptr<PlaybackController> playbackController;
    std::unique_ptr<DisplayController> displayController;
    std::unique_ptr<DataLoadingController> loadingController;
    
    bool userDraggingSlider;
    
    QWidget* centralWidget;
    
    QGroupBox* sensorPanel;
    QGroupBox* imagePanel;
    QGroupBox* mapPanel;
    QGroupBox* controlPanel;
    
    QLabel* gpsLabel;
    QLabel* imuLabel;
    QLabel* speedLabel;
    QLabel* brakeLabel;
    QLabel* throttleLabel;
    QLabel* steeringLabel;
    
    QLabel* frontImageLabel;
    QLabel* backImageLabel;
    QLabel* leftImageLabel;
    QLabel* rightImageLabel;
    
    GPSMapWidget* gpsMapWidget;
    
    QPushButton* loadButton;
    QPushButton* playPauseButton;
    QSlider* timeSlider;
    QLabel* timeLabel;
    QProgressBar* progressBar;
};