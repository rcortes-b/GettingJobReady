#include <QApplication>
#include "MainWindow.h"
#include "TaskManager.h"
#include "Storage.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

static std::string dataPath() {
    const char* home = std::getenv("HOME");
    if (!home) home = "/tmp";
    return std::string(home) + "/.local/share/taskmanager/tasks.json";
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Gestor de Tareas");
    app.setApplicationVersion("1.0");

    try {
        auto storage = std::make_unique<JsonStorage>(dataPath());
        TaskManager manager(std::move(storage));
        MainWindow window(manager);
        window.show();
        return app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
