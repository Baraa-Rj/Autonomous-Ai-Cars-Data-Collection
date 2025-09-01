#pragma once
#include "readers/DataReader.h"
#include "data/IMUData.h"

class IMUDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
    
protected:
    std::unique_ptr<Data> parseLine(const std::string& line) override;
};