#pragma once
#include "readers/DataReader.h"
#include "data/SteeringData.h"

class SteeringDataReader : public DataReader {
public:
    
protected:
    std::unique_ptr<Data> parseLine(const std::string& line) override;
};