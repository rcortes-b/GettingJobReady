#include "CLI.h"
#include "TaskManager.h"
#include "Storage.h"
#include <iostream>
#include <cstdlib>

static std::string dataPath() {
    const char* home = std::getenv("HOME");
    if (!home) home = "/tmp";
    return std::string(home) + "/.local/share/taskmanager/tasks.json";
}

int main(int argc, char* argv[]) {
    try {
        auto storage = std::make_unique<JsonStorage>(dataPath());
        TaskManager manager(std::move(storage));
        CLI cli(manager);
        return cli.run(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
