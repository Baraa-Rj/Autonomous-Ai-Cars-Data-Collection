#pragma once

#include <QWidget>
#include <QTimer>
#include <list>
#include <chrono>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>
#include <QVector>
#include <QString>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../data/Data.h"
#include "../data/DataStore.h"
#include "../sync/ClockManager.h"
#include "../readers/GpsReader.h"
#include "../readers/SpeedReader.h"
#include "../readers/BrakeReader.h"
#include "../readers/ThrottleReader.h"
#include "../readers/SteeringReader.h"
#include "../readers/IMUReader.h"
#include "../readers/ReadersManager.h"


namespace cv { class Mat; }

class DisplayManager : public QWidget {
    Q_OBJECT
protected:
    DataStore* dataStore{nullptr};
    int fps{30};
    std::chrono::system_clock::time_point currentTime;
    QTimer* timer{nullptr};
    ClockManager* clockManager{nullptr};

    
    QLabel *lblGpsLat{nullptr}, *lblGpsLon{nullptr}, *lblGpsAlt{nullptr};
    QLabel *lblSpeed{nullptr}, *lblBrake{nullptr}, *lblThrottle{nullptr}, *lblSteering{nullptr};
    QLabel *lblAccX{nullptr}, *lblAccY{nullptr}, *lblAccZ{nullptr};
    QLabel *lblGyrX{nullptr}, *lblGyrY{nullptr}, *lblGyrZ{nullptr};

    
    enum class Camera { Left, Front, Right, Back };
    QLabel *lblCamLeft{nullptr}, *lblCamFront{nullptr}, *lblCamRight{nullptr}, *lblCamBack{nullptr};
    QPushButton *btnPickLeft{nullptr}, *btnPickFront{nullptr}, *btnPickRight{nullptr}, *btnPickBack{nullptr};
    QLabel *lblPathLeft{nullptr}, *lblPathFront{nullptr}, *lblPathRight{nullptr}, *lblPathBack{nullptr};

    struct CameraFrame { std::chrono::system_clock::time_point ts; QString path; };
    QMap<Camera, QVector<CameraFrame>> cameraFrames;

    
    QLineEdit *edGps{nullptr}, *edSpeed{nullptr}, *edBrake{nullptr}, *edThrottle{nullptr}, *edSteering{nullptr}, *edImu{nullptr};
    QPushButton *btnLoadCsv{nullptr}, *btnPlay{nullptr}, *btnPause{nullptr};
    QLabel *lblTime{nullptr};

    
    std::unique_ptr<GpsReader> gpsReader;
    std::unique_ptr<SpeedReader> speedReader;
    std::unique_ptr<BrakeReader> brakeReader;
    std::unique_ptr<ThrottleReader> throttleReader;
    std::unique_ptr<SteeringReader> steeringReader;
    std::unique_ptr<IMUReader> imuReader;

    std::chrono::system_clock::time_point globalStart;
    std::chrono::system_clock::time_point globalEnd;
public:
    explicit DisplayManager(QWidget* parent = nullptr);
    ~DisplayManager() override = default;
    void displayFrame(std::list<Data> dataItems);
    bool renderData(std::list<Data> dataItems);
    void updateDisplay(std::chrono::system_clock::time_point time);

    void setDataStore(DataStore* dataStore);
    DataStore* getDataStore();

    void setFps(int fps);
    int getFps() const;

public:
    void autoSetupFromSampleData();
    void computeGlobalTimeline();

private:
    void buildUi();
    void connectUi();
    void buildCameraIndex(Camera cam, const QString& dir);
    const CameraFrame* findFrame(const QVector<CameraFrame>& frames, std::chrono::system_clock::time_point t) const;
    void updateCameras(std::chrono::system_clock::time_point t);
    void updateSidebar(std::chrono::system_clock::time_point t);
    static QImage matToQImage(const cv::Mat& mat);
    static void setImageOnLabel(QLabel* lbl, const QString& path);

};


