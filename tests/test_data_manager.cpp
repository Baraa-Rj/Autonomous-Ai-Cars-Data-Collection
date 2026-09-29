// Headless tests for data loading and seeking (no GUI, no display needed).
// The fixture dataset is generated into a temporary directory at runtime.

#include "management/DataManager.h"
#include "readers/DataReaderFactory.h"

#include <opencv2/opencv.hpp>

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

namespace fs = std::filesystem;

static int failures = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK failed: " #cond \
                      << std::endl;                                              \
            ++failures;                                                          \
        }                                                                        \
    } while (0)

#define CHECK_NEAR(actual, expected)                                                        \
    do {                                                                                    \
        double a_ = (actual), e_ = (expected);                                              \
        if (std::fabs(a_ - e_) > 1e-6) {                                                    \
            std::cerr << std::setprecision(17) << __FILE__ << ":" << __LINE__                  \
                      << ": CHECK_NEAR failed: " #actual " = " << a_ << ", expected " << e_ << std::endl;                            \
            ++failures;                                                                     \
        }                                                                                   \
    } while (0)

static const std::vector<std::string> kCameras = {"front", "back", "left", "right"};
static const int kSensorRows = 10;   // sensor rows every 0.5 s
static const int kImagesPerCamera = 5;  // images every 1.0 s

static std::string fmt(double ts) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.6f", ts);
    return buf;
}

// Writes CSVs for every sensor (rows at base + 0.5*i) and small JPEGs per
// camera (frames at base + 1.0*j).
static void writeDataset(const fs::path& dir, double base) {
    fs::create_directories(dir);
    auto writeCsv = [&](const std::string& name, const std::string& header, int extraColumns) {
        std::ofstream out(dir / name);
        out << header << "\n";
        for (int i = 0; i < kSensorRows; ++i) {
            out << fmt(base + 0.5 * i);
            for (int c = 0; c < extraColumns; ++c) {
                out << "," << (i + c);
            }
            out << "\n";
        }
    };
    writeCsv("gps.csv", "time_stamp,latitude,longitude,height", 3);
    writeCsv("imu.csv",
             "time_stamp,x_acc,y_acc,z_acc,pitch,roll,yaw,x_gyro,y_gyro,z_gyro,x_mag,y_mag,z_mag", 12);
    writeCsv("speed.csv", "time_stamp,data_value", 1);
    writeCsv("brake.csv", "time_stamp,data_value", 1);
    writeCsv("throttle.csv", "time_stamp,data_value", 1);
    writeCsv("steering.csv", "time_stamp,data_value", 1);

    for (const auto& camera : kCameras) {
        fs::path camDir = dir / "3d_images" / camera;
        fs::create_directories(camDir);
        for (int j = 0; j < kImagesPerCamera; ++j) {
            cv::Mat img(8, 8, CV_8UC3, cv::Scalar(10 * j, 20, 30));
            cv::imwrite((camDir / (fmt(base + 1.0 * j) + ".jpeg")).string(), img);
        }
    }
}

static void testRowCounts(const fs::path& dir) {
    const std::vector<std::pair<std::string, DataType>> files = {
        {"gps.csv", DataType::GPS},       {"imu.csv", DataType::IMU},
        {"speed.csv", DataType::SPEED},   {"brake.csv", DataType::BRAKE},
        {"throttle.csv", DataType::THROTTLE}, {"steering.csv", DataType::STEERING},
    };
    for (const auto& [name, type] : files) {
        auto reader = DataReaderFactory::createReader(type);
        CHECK(reader && reader->loadAllData((dir / name).string()));
        CHECK(reader && reader->getDataCount() == static_cast<size_t>(kSensorRows));
    }
}

static void checkImagesAt(DataManager& manager, double expected) {
    ImageData* images[] = {manager.getCurrentFrontImage(), manager.getCurrentBackImage(),
                           manager.getCurrentLeftImage(), manager.getCurrentRightImage()};
    for (ImageData* image : images) {
        CHECK(image != nullptr);
        if (image) {
            CHECK_NEAR(image->timestamp, expected);
        }
    }
}

static void testSeeking(const fs::path& dir, double base) {
    DataManager manager;
    CHECK(manager.initializeStreamingReaders(dir.string()));
    CHECK_NEAR(manager.getClockManager().getMinTimestamp(), base);
    CHECK_NEAR(manager.getClockManager().getMaxTimestamp(), base + 0.5 * (kSensorRows - 1));

    // Forward: sensors show the last row <= t, cameras the first frame >= t.
    manager.updateSensorData(base);
    CHECK(manager.getCurrentSpeed() && std::fabs(manager.getCurrentSpeed()->timestamp - base) < 1e-6);
    checkImagesAt(manager, base);

    manager.updateSensorData(base + 1.2);
    CHECK(manager.getCurrentSpeed() && std::fabs(manager.getCurrentSpeed()->timestamp - (base + 1.0)) < 1e-6);
    checkImagesAt(manager, base + 2.0);

    manager.updateSensorData(base + 3.5);
    CHECK(manager.getCurrentSpeed() && std::fabs(manager.getCurrentSpeed()->timestamp - (base + 3.5)) < 1e-6);
    checkImagesAt(manager, base + 4.0);

    // Backward: cameras must follow the clock back, not stay ahead of it.
    manager.updateSensorData(base + 0.7);
    CHECK(manager.getCurrentSpeed() && std::fabs(manager.getCurrentSpeed()->timestamp - (base + 0.5)) < 1e-6);
    checkImagesAt(manager, base + 1.0);

    manager.updateSensorData(base);
    checkImagesAt(manager, base);
}

static void testZeroBasedRange(const fs::path& dir) {
    DataManager manager;
    CHECK(manager.initializeStreamingReaders(dir.string()));
    CHECK_NEAR(manager.getClockManager().getMinTimestamp(), 0.0);
    CHECK_NEAR(manager.getClockManager().getMaxTimestamp(), 0.5 * (kSensorRows - 1));

    ClockManager clock;
    clock.updateRange(0.0);
    clock.updateRange(1.0);
    clock.updateRange(2.0);
    CHECK_NEAR(clock.getMinTimestamp(), 0.0);
    CHECK_NEAR(clock.getMaxTimestamp(), 2.0);
    CHECK(clock.hasValidRange());
}

int main() {
    fs::path root = fs::temp_directory_path() / ("car_status_test_" + std::to_string(::getpid()));
    fs::remove_all(root);

    const double base = 1684926118.0;
    writeDataset(root / "epoch", base);
    writeDataset(root / "zero", 0.0);

    testRowCounts(root / "epoch");
    testSeeking(root / "epoch", base);
    testZeroBasedRange(root / "zero");

    fs::remove_all(root);

    if (failures) {
        std::cerr << failures << " check(s) failed" << std::endl;
        return 1;
    }
    std::cout << "All checks passed" << std::endl;
    return 0;
}
