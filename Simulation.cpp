#include "headers/readers/GpsReader.h"
#include <iostream>
int main() {
    GpsReader reader("/home/dark/Desktop/data-collection-phase/sample_data/gps.csv");
    reader.loadData("/home/dark/Desktop/data-collection-phase/sample_data/gps.csv");
    auto all = reader.getAllData();
    std::cout << "GPS rows: " << all.size() << "\n";
    return 0;
}