/**
 * @file main.cpp
 * @brief Entry point of the application.
 */

#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Apply some styling
    app.setStyle("Fusion");

    MainWindow window;
    window.show();

    return app.exec();
}
