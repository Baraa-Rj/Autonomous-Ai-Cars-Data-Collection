#pragma once

#include "AbstractDataReader.h"
#include <vector>
#include <optional>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

template<typename DataT>
class CSVDataReader : public AbstractDataReader {
private:
    std::vector<DataT> records;
    
    // Trim whitespace from string
    void trim(std::string& s) {
        auto isws = [](int c) { return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](char c) { return !isws(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](char c) { return !isws(c); }).base(), s.end());
    }

protected:
    // Virtual method for subclasses to parse a CSV line into their specific data type
    virtual std::optional<DataT> parseLine(const std::vector<std::string>& tokens) = 0;
    
    // Virtual method to get expected column count (for validation)
    virtual size_t getExpectedColumnCount() const = 0;
    
    // Virtual method to check if this is a header line
    virtual bool isHeaderLine(const std::vector<std::string>& tokens) const {
        return !tokens.empty() && 
               (tokens[0] == "time_stamp" || tokens[0] == "timestamp" || tokens[0] == "time");
    }

public:
    explicit CSVDataReader(const std::string& path) : AbstractDataReader(path) {}
    
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override {
        return AbstractDataReader::getDataAt(time);
    }
    
    void loadData(const std::string& filePath) override {
        items.clear();
        records.clear();
        
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filePath << std::endl;
            return;
        }
        
        std::string line;
        size_t parsed = 0;
        bool firstLine = true;
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            // Parse CSV line into tokens
            std::vector<std::string> tokens;
            std::istringstream iss(line);
            std::string token;
            
            while (std::getline(iss, token, ',')) {
                trim(token);
                tokens.push_back(token);
            }
            
            if (tokens.size() < getExpectedColumnCount()) continue;
            
            // Skip header line if it's the first line and looks like a header
            if (firstLine && isHeaderLine(tokens)) {
                firstLine = false;
                continue;
            }
            firstLine = false;
            
            // Parse the line using the specific data type parser
            auto dataOpt = parseLine(tokens);
            if (dataOpt.has_value()) {
                auto dataPtr = std::make_shared<DataT>(dataOpt.value());
                items.push_back(dataPtr);
                records.push_back(dataOpt.value());
                ++parsed;
            }
        }
        
        std::cout << typeid(DataT).name() << "Reader parsed rows: " << parsed << std::endl;
    }
    
    std::optional<DataT> latestAt(std::chrono::system_clock::time_point t) const {
        if (records.empty()) return std::nullopt;
        
        // Binary search for the latest record at or before time t
        size_t l = 0, r = records.size();
        while (l < r) {
            size_t m = (l + r) / 2;
            if (records[m].getTimestamp() <= t) {
                l = m + 1;
            } else {
                r = m;
            }
        }
        
        if (l == 0) return std::nullopt;
        return records[l - 1];
    }
    
    const std::vector<DataT>& getRecords() const {
        return records;
    }
};