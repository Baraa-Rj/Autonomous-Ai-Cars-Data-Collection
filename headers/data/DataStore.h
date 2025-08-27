#pragma once 
#include "Data.h"
#include <map>
#include <list>

enum class DataType{
    LEFT_IMAGE,
    RIGHT_IMAGE,
    FRONT_IMAGE,
    BACK_IMAGE,
    GPS,
    IMU,
    SPEED,
    STEERING,
    BRAKE,
    THROTTLE
};

class DataStore{
private:
    std::map<DataType,Data> dataItems;

public:
    void addData(DataType type, Data data);
    Data getCurrentDataByType(DataType type);
    std::list<Data> getCurrentData();
};