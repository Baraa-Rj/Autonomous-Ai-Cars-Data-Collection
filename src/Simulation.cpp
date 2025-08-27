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
    
    // LoadersManager now loads all data in its constructor
    readers_manager->loadAllData();
    display->setReadersManager(readers_manager.get());
    display->setTimelineManager(readers_manager->getTimelineManager());
    
    QObject::connect(clock.get(), &ClockManager::ticked, display.get(), &DisplayManager::updateDisplay);
    
    std::cout << "Simulation initialized successfully" << std::endl;
}

void Simulation::run() {
    std::cout << "Starting Simulation..." << std::endl;
    
    display->show();
    clock->start();
    
    std::cout << "Simulation running..." << std::endl;
}
