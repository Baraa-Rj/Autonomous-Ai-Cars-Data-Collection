#include "DisplayManager.h"
#include <opencv2/opencv.hpp>

DisplayManager::DisplayManager(QWidget* parent)
    : QWidget(parent) {
    setWindowTitle("Data Collection Phase");
    resize(800, 600);
    dataStore = new DataStore();
    fps = 30;
    currentTime = std::chrono::system_clock::now();
    clockManager = new ClockManager(this);
    clockManager->setFps(fps);
    clockManager->start();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        
        currentTime += std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::duration<double>(1.0 / static_cast<double>(fps))
        );
        
        updateCameras(currentTime);
        updateSidebar(currentTime);
        this->update();
    });

    buildUi();
    connectUi();
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

void DisplayManager::buildCameraIndex(Camera cam, const QString& dir) {
    QDir d(dir);
    QStringList filters = {"*.jpg", "*.jpeg", "*.png"};
    QFileInfoList files = d.entryInfoList(filters, QDir::Files, QDir::Name);
    QRegularExpression re(R"(^(?<ts>\d+(?:\.\d+)?))");
    QVector<CameraFrame> frames;
    frames.reserve(files.size());
    for (const QFileInfo& fi : files) {
        QString base = fi.completeBaseName();
        auto m = re.match(base);
        if (!m.hasMatch()) continue;
        bool ok = false;
        double secs = m.captured("ts").toDouble(&ok);
        if (!ok) continue;
        auto tp = std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::duration_cast<std::chrono::system_clock::duration>(
                std::chrono::duration<double>(secs))
        );
        frames.push_back({tp, fi.absoluteFilePath()});
    }
    std::sort(frames.begin(), frames.end(), [](const CameraFrame& a, const CameraFrame& b){ return a.ts < b.ts; });
    cameraFrames[cam] = std::move(frames);
}

const DisplayManager::CameraFrame* DisplayManager::findFrame(const QVector<CameraFrame>& frames, std::chrono::system_clock::time_point t) const {
    int l = 0, r = frames.size();
    while (l < r) {
        int m = (l + r) / 2;
        if (frames[m].ts <= t) l = m + 1; else r = m;
    }
    return l ? &frames[l - 1] : nullptr;
}

QImage DisplayManager::matToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
        return img.copy();
    }
    if (mat.type() == CV_8UC1) {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img.copy();
    }
    return {};
}

void DisplayManager::setImageOnLabel(QLabel* lbl, const QString& path) {
    if (!lbl) return;
    cv::Mat m = cv::imread(path.toStdString(), cv::IMREAD_COLOR);
    if (m.empty()) return;
    QImage q = matToQImage(m);
    if (q.isNull()) return;
    QPixmap pm = QPixmap::fromImage(q).scaled(lbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!pm.isNull()) lbl->setPixmap(pm);
}

void DisplayManager::updateCameras(std::chrono::system_clock::time_point t) {
    auto lf = findFrame(cameraFrames[Camera::Left],  t);
    auto ff = findFrame(cameraFrames[Camera::Front], t);
    auto rf = findFrame(cameraFrames[Camera::Right], t);
    auto bf = findFrame(cameraFrames[Camera::Back],  t);
    if (lf && lblCamLeft)  setImageOnLabel(lblCamLeft,  lf->path);
    if (ff && lblCamFront) setImageOnLabel(lblCamFront, ff->path);
    if (rf && lblCamRight) setImageOnLabel(lblCamRight, rf->path);
    if (bf && lblCamBack)  setImageOnLabel(lblCamBack,  bf->path);
}

void DisplayManager::autoSetupFromSampleData() {
    
    QString base = QDir::currentPath() + "/sample_data";
    QString gps = base + "/gps.csv";
    QString speed = base + "/speed.csv";
    QString brake = base + "/brake.csv";
    QString throttle = base + "/throttle.csv";
    QString steering = base + "/steering.csv";
    QString imu = base + "/imu.csv";
    ReadersManager rm;
    if (QFileInfo::exists(gps)) { gpsReader.reset(static_cast<GpsReader*>(rm.createReader(DataType::GPS, gps.toStdString()))); if (gpsReader) gpsReader->loadData(gps.toStdString()); }
    if (QFileInfo::exists(speed)) { speedReader.reset(static_cast<SpeedReader*>(rm.createReader(DataType::SPEED, speed.toStdString()))); if (speedReader) speedReader->loadData(speed.toStdString()); }
    if (QFileInfo::exists(brake)) { brakeReader.reset(static_cast<BrakeReader*>(rm.createReader(DataType::BRAKE, brake.toStdString()))); if (brakeReader) brakeReader->loadData(brake.toStdString()); }
    if (QFileInfo::exists(throttle)) { throttleReader.reset(static_cast<ThrottleReader*>(rm.createReader(DataType::THROTTLE, throttle.toStdString()))); if (throttleReader) throttleReader->loadData(throttle.toStdString()); }
    if (QFileInfo::exists(steering)) { steeringReader.reset(static_cast<SteeringReader*>(rm.createReader(DataType::STEERING, steering.toStdString()))); if (steeringReader) steeringReader->loadData(steering.toStdString()); }
    if (QFileInfo::exists(imu)) { imuReader.reset(static_cast<IMUReader*>(rm.createReader(DataType::IMU, imu.toStdString()))); if (imuReader) imuReader->loadData(imu.toStdString()); }

    
    struct CamEntry { Camera cam; const char* name; } entries[] = {
        {Camera::Left,  "left"},
        {Camera::Front, "front"},
        {Camera::Right, "right"},
        {Camera::Back,  "back"},
    };
    for (const auto& e : entries) {
        QString dir = base + "/3d_images/" + e.name;
        if (QDir(dir).exists()) buildCameraIndex(e.cam, dir);
    }
}

void DisplayManager::updateSidebar(std::chrono::system_clock::time_point t) {
    auto fmt = [](double v){ return QString::number(v, 'f', 3); };
    
    if (gpsReader)   { auto g  = gpsReader->latestAt(t);   if (g)  dataStore->addData(DataType::GPS, *g); }
    if (speedReader) { auto s  = speedReader->latestAt(t); if (s)  dataStore->addData(DataType::SPEED, *s); }
    if (brakeReader) { auto br = brakeReader->latestAt(t); if (br) dataStore->addData(DataType::BRAKE, *br); }
    if (throttleReader){auto th = throttleReader->latestAt(t); if (th) dataStore->addData(DataType::THROTTLE, *th); }
    if (steeringReader){auto st = steeringReader->latestAt(t); if (st) dataStore->addData(DataType::STEERING, *st); }
    if (imuReader)    { auto im = imuReader->latestAt(t); if (im) {} }

    
    
    
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


