#pragma once

#include <string>
#include "../data/DataStore.h"
#include "AbstractDataReader.h"
#include "BrakeReader.h"
#include "GpsReader.h"
#include "ImageReader.h"
#include "IMUReader.h"
#include "SpeedReader.h"
#include "SteeringReader.h"
#include "ThrottleReader.h"

class ReadersManager{
    public:
    ReadersManager() = default;
    ~ReadersManager() = default;
    AbstractDataReader* createReader(DataType type, const std::string& path);
};


