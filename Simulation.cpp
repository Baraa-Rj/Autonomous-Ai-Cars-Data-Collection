#include <QApplication>
#include "headers/gui/DisplayManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    DisplayManager w;
    w.show();
    return app.exec();
}