#pragma once
#include "readers/DataReader.h"
#include "data/BrakeData.h"

class BrakeDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
    
protected:
    std::unique_ptr<Data> parseLine(const std::string& line) override;
};