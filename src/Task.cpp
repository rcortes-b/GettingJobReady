#include "Task.h"
#include <stdexcept>

std::string Task::statusToString(Status s) {
    switch (s) {
        case Status::TODO:        return "todo";
        case Status::IN_PROGRESS: return "in_progress";
        case Status::DONE:        return "done";
    }
    return "todo";
}

std::string Task::priorityToString(Priority p) {
    switch (p) {
        case Priority::LOW:    return "low";
        case Priority::MEDIUM: return "medium";
        case Priority::HIGH:   return "high";
    }
    return "medium";
}

Status Task::statusFromString(const std::string& s) {
    if (s == "in_progress") return Status::IN_PROGRESS;
    if (s == "done")        return Status::DONE;
    return Status::TODO;
}

Priority Task::priorityFromString(const std::string& p) {
    if (p == "low")  return Priority::LOW;
    if (p == "high") return Priority::HIGH;
    return Priority::MEDIUM;
}

nlohmann::json Task::toJson() const {
    return {
        {"id",          id},
        {"title",       title},
        {"description", description},
        {"status",      statusToString(status)},
        {"priority",    priorityToString(priority)},
        {"created_at",  created_at},
        {"due_date",    due_date}
    };
}

Task Task::fromJson(const nlohmann::json& j) {
    Task t;
    t.id          = j.at("id").get<int>();
    t.title       = j.at("title").get<std::string>();
    t.description = j.at("description").get<std::string>();
    t.status      = statusFromString(j.at("status").get<std::string>());
    t.priority    = priorityFromString(j.at("priority").get<std::string>());
    t.created_at  = j.at("created_at").get<std::string>();
    t.due_date    = j.at("due_date").get<std::string>();
    return t;
}
