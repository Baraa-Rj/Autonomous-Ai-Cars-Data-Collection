#include <QApplication>
#include "DisplayManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and show the display manager
    DisplayManager displayManager;
    displayManager.show();

    return app.exec();
}