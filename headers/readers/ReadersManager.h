#pragma once

class ReadersManager{
    public:
    AbstractDataReader* createReader(DataType type, std::string path);
}