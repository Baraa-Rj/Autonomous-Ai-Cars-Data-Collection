#pragma once

#include <memory>
#include "sync/ClockManager.h"
#include "gui/DisplayManager.h"
#include "readers/ReadersManager.h"

class Simulation {
private:
    std::unique_ptr<ClockManager> clock;
    std::unique_ptr<DisplayManager> display;
    std::unique_ptr<ReadersManager> readers_manager;

public:
    Simulation();
    ~Simulation() = default;
    
    void init();
    void run();
};
