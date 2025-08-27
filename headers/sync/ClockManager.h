#pragma once

class ClockManager{
    private:
    std::DateTime currentTime;
    int fps;
    public:
    ClockManager();
    ~ClockManager();
    void start();
    std::DateTime tick();
    std::DateTime getCurrentTime();
    void setCurrentTime(std::DateTime time);
    void setFps(int fps);
    int getFps();
}