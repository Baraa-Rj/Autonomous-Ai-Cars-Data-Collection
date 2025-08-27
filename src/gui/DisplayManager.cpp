#include "DisplayManager.h"
#include "readers/ReadersManager.h"
#include <opencv2/opencv.hpp>

DisplayManager::DisplayManager(QWidget* parent)
    : QWidget(parent), readersManager(nullptr) {
    setWindowTitle("Data Collection Phase");
    resize(800, 600);
    dataStore = new DataStore();
    currentTime = std::chrono::system_clock::now();

    buildUi();
}


void DisplayManager::buildUi() {
    auto* mainLayout = new QHBoxLayout(this);

    
    auto* side = new QWidget(this);
    auto* sideLayout = new QVBoxLayout(side);
    lblGpsLat = new QLabel("GPS Lat: -", side);
    lblGpsLon = new QLabel("GPS Lon: -", side);
    lblGpsAlt = new QLabel("GPS Alt: -", side);
    lblSpeed = new QLabel("Speed: -", side);
    lblBrake = new QLabel("Brake: -", side);
    lblThrottle = new QLabel("Throttle: -", side);
    lblSteering = new QLabel("Steering: -", side);
    lblAccX = new QLabel("Acc X: -", side);
    lblAccY = new QLabel("Acc Y: -", side);
    lblAccZ = new QLabel("Acc Z: -", side);
    lblGyrX = new QLabel("Gyro X: -", side);
    lblGyrY = new QLabel("Gyro Y: -", side);
    lblGyrZ = new QLabel("Gyro Z: -", side);
    lblTime = new QLabel("t: -", side);
    sideLayout->addWidget(lblGpsLat);
    sideLayout->addWidget(lblGpsLon);
    sideLayout->addWidget(lblGpsAlt);
    sideLayout->addWidget(lblSpeed);
    sideLayout->addWidget(lblBrake);
    sideLayout->addWidget(lblThrottle);
    sideLayout->addWidget(lblSteering);
    sideLayout->addWidget(lblAccX);
    sideLayout->addWidget(lblAccY);
    sideLayout->addWidget(lblAccZ);
    sideLayout->addWidget(lblGyrX);
    sideLayout->addWidget(lblGyrY);
    sideLayout->addWidget(lblGyrZ);
    sideLayout->addWidget(lblTime);
    sideLayout->addStretch();

    
    auto* central = new QWidget(this);
    auto* centralLayout = new QVBoxLayout(central);

    auto* grid = new QGridLayout();
    lblCamLeft = new QLabel("Left", central); lblCamLeft->setAlignment(Qt::AlignCenter);
    lblCamFront = new QLabel("Front", central); lblCamFront->setAlignment(Qt::AlignCenter);
    lblCamRight = new QLabel("Right", central); lblCamRight->setAlignment(Qt::AlignCenter);
    lblCamBack = new QLabel("Back", central); lblCamBack->setAlignment(Qt::AlignCenter);
    lblCamLeft->setMinimumSize(320, 180);
    lblCamFront->setMinimumSize(320, 180);
    lblCamRight->setMinimumSize(320, 180);
    lblCamBack->setMinimumSize(320, 180);
    grid->addWidget(lblCamLeft, 0, 0);
    grid->addWidget(lblCamFront, 0, 1);
    grid->addWidget(lblCamRight, 1, 0);
    grid->addWidget(lblCamBack, 1, 1);

    
    centralLayout->addLayout(grid);

    mainLayout->addWidget(side, 0);
    mainLayout->addWidget(central, 1);
    setLayout(mainLayout);
}

void DisplayManager::connectUi() {
    // TODO: Implement connection logic
    // autoSetupFromSampleData();
    // computeGlobalTimeline();
    currentTime = globalStart;
    updateSidebar(currentTime);
    // TODO: Fix camera frames logic
}



void DisplayManager::updateCameras(std::chrono::system_clock::time_point t) {
    if (!readersManager) return;
    
    // TODO: Fix image reader methods - latestAt doesn't exist yet
    /*
    auto updateCamera = [&](DataType imageType, QLabel* label) {
        auto imageReader = static_cast<ImageReader*>(readersManager->getReader(imageType));
        if (imageReader) {
            auto imageData = imageReader->latestAt(t);
            if (imageData) {
                ImageHandler::setImageOnLabel(label, QString::fromStdString(imageData->getPath()));
            }
        }
    };

    updateCamera(DataType::LEFT_IMAGE, lblCamLeft);
    updateCamera(DataType::FRONT_IMAGE, lblCamFront);
    updateCamera(DataType::RIGHT_IMAGE, lblCamRight);
    updateCamera(DataType::BACK_IMAGE, lblCamBack);
    */
}

void DisplayManager::setReadersManager(ReadersManager* rm) {
    readersManager = rm;
}

void DisplayManager::initializeTimeline() {
    if (readersManager) {
        globalStart = readersManager->getGlobalStart();
        globalEnd = readersManager->getGlobalEnd();
        currentTime = globalStart;
    }
}

void DisplayManager::updateSidebar(std::chrono::system_clock::time_point t) {
    if (!readersManager) return;
    
    auto fmt = [](double v){ return QString::number(v, 'f', 3); };
    
    // Update DataStore with latest data
    auto gpsReader = static_cast<GpsReader*>(readersManager->getReader(DataType::GPS));
    auto speedReader = static_cast<SpeedReader*>(readersManager->getReader(DataType::SPEED));
    auto brakeReader = static_cast<BrakeReader*>(readersManager->getReader(DataType::BRAKE));
    auto throttleReader = static_cast<ThrottleReader*>(readersManager->getReader(DataType::THROTTLE));
    auto steeringReader = static_cast<SteeringReader*>(readersManager->getReader(DataType::STEERING));
    auto imuReader = static_cast<IMUReader*>(readersManager->getReader(DataType::IMU));
    
    // TODO: Fix DataStore usage - need to create copies or change design
    // Temporarily commenting out since DataStore expects unique_ptr ownership
    // if (gpsReader)   { auto g  = gpsReader->latestAt(t);   if (g)  dataStore->addData(DataType::GPS, std::make_unique<GpsData>(*g)); }
    // if (speedReader) { auto s  = speedReader->latestAt(t); if (s)  dataStore->addData(DataType::SPEED, std::make_unique<SpeedData>(*s)); }

    // Update GUI labels - TODO: Fix latestAt method calls
    /*
    if (gpsReader) {
        auto g = gpsReader->latestAt(t);
        if (g) {
            lblGpsLat->setText("GPS Lat: " + fmt(g->getLatitude()));
            lblGpsLon->setText("GPS Lon: " + fmt(g->getLongitude()));
            lblGpsAlt->setText("GPS Alt: " + fmt(g->getAltitude()));
        }
    }
    */
    // Temporary placeholder values
    lblGpsLat->setText("GPS Lat: -");
    lblGpsLon->setText("GPS Lon: -");
    lblGpsAlt->setText("GPS Alt: -");
    lblSpeed->setText("Speed: -");
    lblBrake->setText("Brake: -");
    lblThrottle->setText("Throttle: -");
    lblSteering->setText("Steering: -");
    lblAccX->setText("Acc X: -");
    lblAccY->setText("Acc Y: -");
    lblAccZ->setText("Acc Z: -");
    lblGyrX->setText("Gyro X: -");
    lblGyrY->setText("Gyro Y: -");
    lblGyrZ->setText("Gyro Z: -");

    // Update time display
    auto secs = std::chrono::duration<double>(t.time_since_epoch()).count();
    lblTime->setText(QString("t: %1").arg(QString::number(secs, 'f', 3)));
}

void DisplayManager::computeGlobalTimeline() {
    // TODO: Fix timeline computation - getAllData returns unique_ptr now
    globalStart = std::chrono::system_clock::now();
    globalEnd = globalStart + std::chrono::seconds(60);  // 1 minute default
}

void DisplayManager::displayFrame(std::list<std::shared_ptr<Data>> dataItems) {
    for (const auto& data : dataItems) {
        dataStore->addData(data->getType(), data);
    }
    updateDisplay(currentTime);
}

bool DisplayManager::renderData(std::list<std::shared_ptr<Data>> dataItems) {
    displayFrame(dataItems);
    return true;
}

void DisplayManager::updateDisplay(std::chrono::system_clock::time_point time) {
    currentTime = time;
    updateCameras(currentTime);
    updateSidebar(currentTime);
    this->update();
}

void DisplayManager::setDataStore(DataStore* ds) {
    dataStore = ds;
}

DataStore* DisplayManager::getDataStore() {
    return dataStore;
}

void DisplayManager::setFps(int f) {
    fps = f;
}

int DisplayManager::getFps() const {
    return fps;
}