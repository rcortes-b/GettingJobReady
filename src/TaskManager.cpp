#include "TaskManager.h"
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

static std::string currentTimestamp() {
    auto now  = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

TaskManager::TaskManager(std::unique_ptr<Storage> storage)
    : storage_(std::move(storage)) {
    tasks_ = storage_->load();
}

int TaskManager::nextId() const {
    int max = 0;
    for (const auto& t : tasks_)
        if (t.id > max) max = t.id;
    return max + 1;
}

void TaskManager::persist() {
    storage_->save(tasks_);
}

Task TaskManager::add(const std::string& title, const std::string& description,
                      Priority priority, const std::string& due_date) {
    Task t;
    t.id          = nextId();
    t.title       = title;
    t.description = description;
    t.status      = Status::TODO;
    t.priority    = priority;
    t.created_at  = currentTimestamp();
    t.due_date    = due_date;
    tasks_.push_back(t);
    persist();
    return t;
}

bool TaskManager::remove(int id) {
    auto it = std::find_if(tasks_.begin(), tasks_.end(),
                           [id](const Task& t) { return t.id == id; });
    if (it == tasks_.end()) return false;
    tasks_.erase(it);
    persist();
    return true;
}

bool TaskManager::updateStatus(int id, Status status) {
    for (auto& t : tasks_) {
        if (t.id == id) { t.status = status; persist(); return true; }
    }
    return false;
}

bool TaskManager::updateTitle(int id, const std::string& title) {
    for (auto& t : tasks_) {
        if (t.id == id) { t.title = title; persist(); return true; }
    }
    return false;
}

std::optional<Task> TaskManager::get(int id) const {
    for (const auto& t : tasks_)
        if (t.id == id) return t;
    return std::nullopt;
}

std::vector<Task> TaskManager::listAll() const {
    return tasks_;
}

std::vector<Task> TaskManager::filter(std::optional<Status>   status,
                                       std::optional<Priority> priority) const {
    std::vector<Task> result;
    for (const auto& t : tasks_) {
        if (status   && t.status   != *status)   continue;
        if (priority && t.priority != *priority) continue;
        result.push_back(t);
    }
    return result;
}
