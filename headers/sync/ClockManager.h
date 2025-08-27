#pragma once

#include <QObject>
#include <QTimer>
#include <chrono>

class ClockManager : public QObject {
    Q_OBJECT

private:
    std::chrono::system_clock::time_point currentTime;
    int fps{30};
    QTimer* timer{nullptr};

public:
    explicit ClockManager(QObject* parent = nullptr);
    ~ClockManager() override;

    void start();
    std::chrono::system_clock::time_point tick();
    std::chrono::system_clock::time_point getCurrentTime() const;
    void setCurrentTime(std::chrono::system_clock::time_point time);
    void setFps(int fps);
    int getFps() const;
};