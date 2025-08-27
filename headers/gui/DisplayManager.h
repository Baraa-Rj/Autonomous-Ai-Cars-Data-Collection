#pragma once

#include <QWidget>

class DisplayManager : public QWidget {
    Q_OBJECT
protected:
    DataStore* dataStore;
public:
    explicit DisplayManager(QWidget* parent = nullptr);
    ~DisplayManager() override = default;
    void displayFrame(std::list<Data> dataItems);
    bool rederData(std::list<Data> dataItems);

    void setDataStore(DataStore* dataStore);
    DataStore* getDataStore();

    void setFps(int fps);
    int getFps();

};


