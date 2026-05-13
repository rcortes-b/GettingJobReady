#pragma once
#include <string>
#include <nlohmann/json.hpp>

enum class Status   { TODO, IN_PROGRESS, DONE };
enum class Priority { LOW, MEDIUM, HIGH };

struct Task {
    int         id;
    std::string title;
    std::string description;
    Status      status;
    Priority    priority;
    std::string created_at;
    std::string due_date;

    nlohmann::json toJson() const;
    static Task    fromJson(const nlohmann::json& j);

    static std::string statusToString(Status s);
    static std::string priorityToString(Priority p);
    static Status      statusFromString(const std::string& s);
    static Priority    priorityFromString(const std::string& p);
};
