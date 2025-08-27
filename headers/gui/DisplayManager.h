#pragma once

#include <QWidget>
#include <QTimer>
#include <list>
#include <chrono>

#include "../data/Data.h"
#include "../data/DataStore.h"
#include "../sync/ClockManager.h"

class DisplayManager : public QWidget {
    Q_OBJECT
protected:
    DataStore* dataStore{nullptr};
    int fps{30};
    std::chrono::system_clock::time_point currentTime;
    QTimer* timer{nullptr};
    ClockManager* clockManager{nullptr};
public:
    explicit DisplayManager(QWidget* parent = nullptr);
    ~DisplayManager() override = default;
    void displayFrame(std::list<Data> dataItems);
    bool renderData(std::list<Data> dataItems);

    void setDataStore(DataStore* dataStore);
    DataStore* getDataStore();

    void setFps(int fps);
    int getFps() const;

};


