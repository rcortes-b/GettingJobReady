#pragma once
#include "Task.h"
#include "Storage.h"
#include <vector>
#include <memory>
#include <optional>

class TaskManager {
public:
    explicit TaskManager(std::unique_ptr<Storage> storage);

    Task add(const std::string& title,
             const std::string& description = "",
             Priority priority = Priority::MEDIUM,
             const std::string& due_date = "");

    bool remove(int id);
    bool updateStatus(int id, Status status);
    bool updateTitle(int id, const std::string& title);

    std::optional<Task>  get(int id) const;
    std::vector<Task>    listAll() const;
    std::vector<Task>    filter(std::optional<Status>   status,
                                std::optional<Priority> priority) const;

private:
    std::unique_ptr<Storage> storage_;
    std::vector<Task>        tasks_;

    int  nextId() const;
    void persist();
};
