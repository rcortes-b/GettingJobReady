#include "CLI.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

CLI::CLI(TaskManager& manager) : manager_(manager) {}

int CLI::run(int argc, char* argv[]) {
    if (argc < 2) { printHelp(); return 0; }

    std::vector<std::string> args(argv + 1, argv + argc);
    const std::string& cmd = args[0];

    if (cmd == "add")                        return cmdAdd(args);
    if (cmd == "list" || cmd == "ls")        return cmdList(args);
    if (cmd == "done")                       return cmdDone(args);
    if (cmd == "delete" || cmd == "rm")      return cmdDelete(args);
    if (cmd == "show")                       return cmdShow(args);
    if (cmd == "edit")                       return cmdEdit(args);
    if (cmd == "help" || cmd == "--help")    { printHelp(); return 0; }

    std::cerr << "Unknown command: " << cmd << "\n";
    printHelp();
    return 1;
}

std::string CLI::getFlag(const std::vector<std::string>& args,
                         const std::string& flag) {
    for (size_t i = 1; i + 1 < args.size(); ++i)
        if (args[i] == flag) return args[i + 1];
    return "";
}

bool CLI::hasFlag(const std::vector<std::string>& args,
                  const std::string& flag) {
    return std::find(args.begin(), args.end(), flag) != args.end();
}

int CLI::cmdAdd(const std::vector<std::string>& args) {
    if (args.size() < 2 || args[1].rfind("--", 0) == 0) {
        std::cerr << "Usage: task add <title> [--desc <text>] "
                     "[--priority low|medium|high] [--due YYYY-MM-DD]\n";
        return 1;
    }
    std::string title       = args[1];
    std::string desc        = getFlag(args, "--desc");
    std::string due         = getFlag(args, "--due");
    std::string priorityStr = getFlag(args, "--priority");

    Priority priority = priorityStr.empty()
                      ? Priority::MEDIUM
                      : Task::priorityFromString(priorityStr);

    Task t = manager_.add(title, desc, priority, due);
    std::cout << "Added task #" << t.id << ": " << t.title << "\n";
    return 0;
}

int CLI::cmdList(const std::vector<std::string>& args) {
    std::optional<Status>   statusFilter;
    std::optional<Priority> priorityFilter;

    std::string s = getFlag(args, "--status");
    std::string p = getFlag(args, "--priority");
    if (!s.empty()) statusFilter   = Task::statusFromString(s);
    if (!p.empty()) priorityFilter = Task::priorityFromString(p);

    auto tasks = manager_.filter(statusFilter, priorityFilter);
    if (tasks.empty()) { std::cout << "No tasks found.\n"; return 0; }
    printTaskTable(tasks);
    return 0;
}

int CLI::cmdDone(const std::vector<std::string>& args) {
    if (args.size() < 2) { std::cerr << "Usage: task done <id>\n"; return 1; }
    int id = std::stoi(args[1]);
    if (!manager_.updateStatus(id, Status::DONE)) {
        std::cerr << "Task #" << id << " not found.\n"; return 1;
    }
    std::cout << "Task #" << id << " marked as done.\n";
    return 0;
}

int CLI::cmdDelete(const std::vector<std::string>& args) {
    if (args.size() < 2) { std::cerr << "Usage: task delete <id>\n"; return 1; }
    int id = std::stoi(args[1]);
    if (!manager_.remove(id)) {
        std::cerr << "Task #" << id << " not found.\n"; return 1;
    }
    std::cout << "Deleted task #" << id << ".\n";
    return 0;
}

int CLI::cmdShow(const std::vector<std::string>& args) {
    if (args.size() < 2) { std::cerr << "Usage: task show <id>\n"; return 1; }
    int id = std::stoi(args[1]);
    auto task = manager_.get(id);
    if (!task) { std::cerr << "Task #" << id << " not found.\n"; return 1; }
    printTask(*task);
    return 0;
}

int CLI::cmdEdit(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: task edit <id> [--title <title>] "
                     "[--status todo|in_progress|done]\n";
        return 1;
    }
    int id = std::stoi(args[1]);
    bool changed = false;

    std::string title     = getFlag(args, "--title");
    std::string statusStr = getFlag(args, "--status");

    if (!title.empty()) {
        if (!manager_.updateTitle(id, title)) {
            std::cerr << "Task #" << id << " not found.\n"; return 1;
        }
        changed = true;
    }
    if (!statusStr.empty()) {
        if (!manager_.updateStatus(id, Task::statusFromString(statusStr))) {
            std::cerr << "Task #" << id << " not found.\n"; return 1;
        }
        changed = true;
    }
    if (!changed) { std::cerr << "No changes specified.\n"; return 1; }
    std::cout << "Task #" << id << " updated.\n";
    return 0;
}

void CLI::printHelp() const {
    std::cout <<
        "Usage: task <command> [options]\n\n"
        "Commands:\n"
        "  add <title> [--desc <text>] [--priority low|medium|high] [--due YYYY-MM-DD]\n"
        "  list        [--status todo|in_progress|done] [--priority low|medium|high]\n"
        "  done <id>\n"
        "  show <id>\n"
        "  edit <id>   [--title <text>] [--status todo|in_progress|done]\n"
        "  delete <id>\n"
        "  help\n";
}

void CLI::printTask(const Task& t) const {
    std::cout
        << "ID:          " << t.id << "\n"
        << "Title:       " << t.title << "\n"
        << "Description: " << t.description << "\n"
        << "Status:      " << Task::statusToString(t.status) << "\n"
        << "Priority:    " << Task::priorityToString(t.priority) << "\n"
        << "Created:     " << t.created_at << "\n"
        << "Due:         " << (t.due_date.empty() ? "(none)" : t.due_date) << "\n";
}

void CLI::printTaskTable(const std::vector<Task>& tasks) const {
    std::cout << std::left
              << std::setw(5)  << "ID"
              << std::setw(30) << "Title"
              << std::setw(13) << "Status"
              << std::setw(10) << "Priority"
              << "Due\n"
              << std::string(67, '-') << "\n";
    for (const auto& t : tasks) {
        std::string title = t.title.size() > 28
                          ? t.title.substr(0, 25) + "..."
                          : t.title;
        std::cout << std::left
                  << std::setw(5)  << t.id
                  << std::setw(30) << title
                  << std::setw(13) << Task::statusToString(t.status)
                  << std::setw(10) << Task::priorityToString(t.priority)
                  << (t.due_date.empty() ? "" : t.due_date) << "\n";
    }
}
