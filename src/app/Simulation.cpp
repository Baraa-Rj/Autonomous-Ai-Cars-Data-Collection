#include <QtWidgets/QApplication>
#include "gui/MainWindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Car Visualization");    
    try {
        MainWindow window;
        window.show();
        
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}