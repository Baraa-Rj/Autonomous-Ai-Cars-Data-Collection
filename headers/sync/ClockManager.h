#ifndef CLOCKMANAGER_H
#define CLOCKMANAGER_H

#include <QObject>
#include <chrono>
#include <thread>
#include <atomic>

class ClockManager : public QObject {
    Q_OBJECT

public:
    explicit ClockManager(QObject* parent = nullptr);
    ~ClockManager();

    void start();
    void stop();
    std::chrono::system_clock::time_point tick();
    
    void setFps(int fps);
    int getFps() const;
    
    void setCurrentTime(std::chrono::system_clock::time_point time);
    std::chrono::system_clock::time_point getCurrentTime() const;

signals:
    void timeUpdated(); // Optional signal for Qt integration

private:
    int fps;
    std::chrono::system_clock::time_point currentTime;
    std::thread timingThread;
    std::atomic<bool> running;
};

#endif // CLOCKMANAGER_H