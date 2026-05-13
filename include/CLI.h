#pragma once
#include "TaskManager.h"
#include <string>
#include <vector>

class CLI {
public:
    explicit CLI(TaskManager& manager);
    int run(int argc, char* argv[]);

private:
    TaskManager& manager_;

    int cmdAdd(const std::vector<std::string>& args);
    int cmdList(const std::vector<std::string>& args);
    int cmdDone(const std::vector<std::string>& args);
    int cmdDelete(const std::vector<std::string>& args);
    int cmdShow(const std::vector<std::string>& args);
    int cmdEdit(const std::vector<std::string>& args);

    void printHelp() const;
    void printTask(const Task& t) const;
    void printTaskTable(const std::vector<Task>& tasks) const;

    static std::string getFlag(const std::vector<std::string>& args,
                               const std::string& flag);
    static bool        hasFlag(const std::vector<std::string>& args,
                               const std::string& flag);
};
