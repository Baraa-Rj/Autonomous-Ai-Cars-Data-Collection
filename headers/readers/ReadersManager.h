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
    ReadersManager();
    ~ReadersManager();
    AbstractDataReader* createReader(DataType type, std::string path);
};