#include "data/DataStore.h"

void DataStore::addData(DataType type, std::shared_ptr<Data> data) {
    dataItems.insert_or_assign(type, data);
}

std::shared_ptr<Data> DataStore::getCurrentDataByType(DataType type) {
    auto it = dataItems.find(type);
    return (it != dataItems.end()) ? it->second : nullptr;
}

std::list<std::shared_ptr<Data>> DataStore::getCurrentData() {
    std::list<std::shared_ptr<Data>> result;
    for (const auto& pair : dataItems) {
        result.push_back(pair.second);
    }
    return result;
}