#pragma once

#include <QWidget>
#include <chrono>
#include <memory>
#include "data/DataStore.h"
#include "data/Data.h"
#include "gui/UIBuilder.h"
#include "gui/DataPresenter.h"

class ReadersManager;
class TimelineManager;

class DisplayManager : public QWidget {
    Q_OBJECT
    
private:
    DataStore* dataStore;
    ReadersManager* readersManager;
    TimelineManager* timelineManager;
    std::chrono::system_clock::time_point currentTime;
    int fps{30};
    
    // Composition - single responsibility components
    UIComponents uiComponents;
    std::unique_ptr<DataPresenter> dataPresenter;
    
public:
    explicit DisplayManager(QWidget* parent = nullptr);
    ~DisplayManager() override = default;
    
    void displayFrame(std::list<std::shared_ptr<Data>> dataItems);
    bool renderData(std::list<std::shared_ptr<Data>> dataItems);
    
    void setDataStore(DataStore* dataStore);
    DataStore* getDataStore();
    
    void setFps(int fps);
    int getFps() const;
    
    void setReadersManager(ReadersManager* rm);
    void setTimelineManager(TimelineManager* tm);
    
public slots:
    void updateDisplay(std::chrono::system_clock::time_point time);
};