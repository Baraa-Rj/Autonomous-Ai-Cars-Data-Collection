#pragma once
#include "readers/DataReader.h"
#include "data/ThrottleData.h"

class ThrottleDataReader : public DataReader {
public:
    
protected:
    std::unique_ptr<Data> parseLine(const std::string& line) override;
};