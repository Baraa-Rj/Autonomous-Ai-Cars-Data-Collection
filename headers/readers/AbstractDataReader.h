#pragma once

class AbstractDataReader{
    protected:
    std::string path;
    public:
    void loadData(std::string path) = 0 ;
    std::list<Data> getDataAt(std::DateTime time);
}