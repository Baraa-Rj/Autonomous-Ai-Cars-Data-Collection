#include "Simulation.h"
#include <QApplication>
#include <QObject>
#include <iostream>

Simulation::Simulation() {
    clock = std::make_unique<ClockManager>();
    display = std::make_unique<DisplayManager>();
    readers_manager = std::make_unique<ReadersManager>();
}

void Simulation::init() {
    std::cout << "Initializing Simulation..." << std::endl;
    
    clock->setFps(30);
    
    display->autoSetupFromSampleData();
    display->computeGlobalTimeline();
    
    connect(clock.get(), &ClockManager::ticked, display.get(), &DisplayManager::updateDisplay);
    
    std::cout << "Simulation initialized successfully" << std::endl;
}

void Simulation::run() {
    std::cout << "Starting Simulation..." << std::endl;
    
    display->show();
    clock->start();
    
    std::cout << "Simulation running..." << std::endl;
}
