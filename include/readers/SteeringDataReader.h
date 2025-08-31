#pragma once
#include "readers/DataReader.h"
#include "data/SteeringData.h"

class SteeringDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
};