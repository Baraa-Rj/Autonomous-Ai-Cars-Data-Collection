#include "headers/data/DataStore.h"

DataStore::DataStore() {}

DataStore::~DataStore() {}

void DataStore::addData(DataType type, Data data) {
    dataItems[type] = data;
}