#pragma once 
#include "Data.h"
#include <map>
#include <list>
#include <memory>

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
    std::map<DataType,std::shared_ptr<Data>> dataItems;

public:
    void addData(DataType type, std::shared_ptr<Data> data);
    std::shared_ptr<Data> getCurrentDataByType(DataType type);
    std::list<std::shared_ptr<Data>> getCurrentData();
};