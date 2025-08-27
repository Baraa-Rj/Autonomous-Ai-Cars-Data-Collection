#pragma once

#include <QObject>
#include <chrono>
#include <thread>
#include <atomic>

class ClockManager : public QObject {
    Q_OBJECT

private:
    std::chrono::system_clock::time_point currentTime;
    int fps{30};
    std::thread loopThread;
    std::atomic<bool> running{false};

public:
    explicit ClockManager(QObject* parent = nullptr);
    ~ClockManager() override;

    void start();
    void stop();
    std::chrono::system_clock::time_point tick();
    std::chrono::system_clock::time_point getCurrentTime() const;
    void setCurrentTime(std::chrono::system_clock::time_point time);
    void setFps(int fps);
    int getFps() const;

signals:
    void ticked(std::chrono::system_clock::time_point t);
};