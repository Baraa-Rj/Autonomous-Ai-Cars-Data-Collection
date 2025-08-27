#include "DisplayManager.h"
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
    
    autoSetupFromSampleData();
    computeGlobalTimeline();
    currentTime = globalStart;
    updateSidebar(currentTime);
    bool hasAnyFrames =
        !cameraFrames[Camera::Left].isEmpty() ||
        !cameraFrames[Camera::Front].isEmpty() ||
        !cameraFrames[Camera::Right].isEmpty() ||
        !cameraFrames[Camera::Back].isEmpty();
    if (hasAnyFrames) {
        timer->start(static_cast<int>(1000.0 / static_cast<double>(fps)));
    }
}



void DisplayManager::updateCameras(std::chrono::system_clock::time_point t) {
    if (!readersManager) return;
    
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
    
    if (gpsReader)   { auto g  = gpsReader->latestAt(t);   if (g)  dataStore->addData(DataType::GPS, *g); }
    if (speedReader) { auto s  = speedReader->latestAt(t); if (s)  dataStore->addData(DataType::SPEED, *s); }
    if (brakeReader) { auto br = brakeReader->latestAt(t); if (br) dataStore->addData(DataType::BRAKE, *br); }
    if (throttleReader){auto th = throttleReader->latestAt(t); if (th) dataStore->addData(DataType::THROTTLE, *th); }
    if (steeringReader){auto st = steeringReader->latestAt(t); if (st) dataStore->addData(DataType::STEERING, *st); }
    if (imuReader)    { auto im = imuReader->latestAt(t); if (im) dataStore->addData(DataType::IMU, *im); }

    // Update GUI labels
    if (gpsReader) {
        auto g = gpsReader->latestAt(t);
        if (g) {
            lblGpsLat->setText("GPS Lat: " + fmt(g->getLatitude()));
            lblGpsLon->setText("GPS Lon: " + fmt(g->getLongitude()));
            lblGpsAlt->setText("GPS Alt: " + fmt(g->getAltitude()));
        }
    }
    if (speedReader) {
        auto s = speedReader->latestAt(t);
        if (s) lblSpeed->setText("Speed: " + fmt(s->getSpeed()));
    }
    if (brakeReader) {
        auto b = brakeReader->latestAt(t);
        if (b) lblBrake->setText("Brake: " + fmt(b->getPressure()));
    }
    if (throttleReader) {
        auto th = throttleReader->latestAt(t);
        if (th) lblThrottle->setText("Throttle: " + fmt(th->getPosition()));
    }
    if (steeringReader) {
        auto st = steeringReader->latestAt(t);
        if (st) lblSteering->setText("Steering: " + fmt(st->getAngle()));
    }
    if (imuReader) {
        auto im = imuReader->latestAt(t);
        if (im) {
            auto acc = im->getAcceleration();
            auto gyr = im->getGyroscope();
            if (acc.size() >= 3) {
                lblAccX->setText("Acc X: " + fmt(acc[0]));
                lblAccY->setText("Acc Y: " + fmt(acc[1]));
                lblAccZ->setText("Acc Z: " + fmt(acc[2]));
            }
            if (gyr.size() >= 3) {
                lblGyrX->setText("Gyro X: " + fmt(gyr[0]));
                lblGyrY->setText("Gyro Y: " + fmt(gyr[1]));
                lblGyrZ->setText("Gyro Z: " + fmt(gyr[2]));
            }
        }
    }

    // Update time display
    auto secs = std::chrono::duration<double>(t.time_since_epoch()).count();
    lblTime->setText(QString("t: %1").arg(QString::number(secs, 'f', 3)));
}

void DisplayManager::computeGlobalTimeline() {
    bool init = false;
    auto consider = [&](const auto& vec){
        if (vec.empty()) return;
        auto s = vec.front().getTimestamp();
        auto e = vec.back().getTimestamp();
        if (!init) { globalStart = s; globalEnd = e; init = true; return; }
        if (s < globalStart) globalStart = s;
        if (e > globalEnd) globalEnd = e;
    };
    if (gpsReader) consider(gpsReader->getAllData());
    if (speedReader) consider(speedReader->getAllData());
    if (brakeReader) consider(brakeReader->getAllData());
    if (throttleReader) consider(throttleReader->getAllData());
    if (steeringReader) consider(steeringReader->getAllData());
}

void DisplayManager::displayFrame(std::list<Data> dataItems) {
    for (const auto& data : dataItems) {
        dataStore->addData(data.getType(), data);
    }
    updateDisplay(currentTime);
}

bool DisplayManager::renderData(std::list<Data> dataItems) {
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