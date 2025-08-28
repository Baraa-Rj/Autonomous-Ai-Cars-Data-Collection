#include <QtWidgets/QApplication>
#include "gui/DisplayManager.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Car Visualization");    
    try {
        DisplayManager window;
        window.show();
        
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}