#include "data/DataStore.h"

void DataStore::addData(DataType type, Data data) {
    dataItems.insert_or_assign(type, data);
}