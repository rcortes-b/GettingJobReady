#include "Storage.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <stdexcept>

JsonStorage::JsonStorage(const std::string& path) : path_(path) {}

void JsonStorage::ensureDir() const {
    auto dir = std::filesystem::path(path_).parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir))
        std::filesystem::create_directories(dir);
}

std::vector<Task> JsonStorage::load() {
    if (!std::filesystem::exists(path_))
        return {};

    std::ifstream file(path_);
    if (!file.is_open())
        throw std::runtime_error("Cannot open: " + path_);

    nlohmann::json j;
    file >> j;

    std::vector<Task> tasks;
    for (const auto& item : j)
        tasks.push_back(Task::fromJson(item));
    return tasks;
}

void JsonStorage::save(const std::vector<Task>& tasks) {
    ensureDir();

    nlohmann::json j = nlohmann::json::array();
    for (const auto& t : tasks)
        j.push_back(t.toJson());

    std::ofstream file(path_);
    if (!file.is_open())
        throw std::runtime_error("Cannot write: " + path_);
    file << j.dump(2);
}
