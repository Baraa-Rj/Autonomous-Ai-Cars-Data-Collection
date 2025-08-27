#pragma once

#include <memory>
#include <string>
#include "../data/DataStore.h"

class AbstractDataReader;

class ReaderFactory {
public:
    static std::unique_ptr<AbstractDataReader> createReader(DataType type, const std::string& path);
    
private:
    ReaderFactory() = default;  // Prevent instantiation
};