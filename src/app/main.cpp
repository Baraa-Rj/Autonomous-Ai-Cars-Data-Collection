#include <QtWidgets/QApplication>
#include "gui/MainWindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Car Status Visualization");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Vehicle Data Systems");
    
    try {
        MainWindow window;
        window.show();
        
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}