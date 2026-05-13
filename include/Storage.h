#pragma once
#include "Task.h"
#include <vector>
#include <string>

class Storage {
public:
    virtual ~Storage() = default;
    virtual std::vector<Task> load() = 0;
    virtual void save(const std::vector<Task>& tasks) = 0;
};

class JsonStorage : public Storage {
public:
    explicit JsonStorage(const std::string& path);
    std::vector<Task> load() override;
    void save(const std::vector<Task>& tasks) override;

private:
    std::string path_;
    void ensureDir() const;
};
