#include <gtest/gtest.h>
#include "Task.h"

TEST(TaskTest, SerializationRoundtrip) {
    Task t;
    t.id          = 1;
    t.title       = "Test task";
    t.description = "A description";
    t.status      = Status::IN_PROGRESS;
    t.priority    = Priority::HIGH;
    t.created_at  = "2026-05-13 10:00:00";
    t.due_date    = "2026-06-01";

    Task t2 = Task::fromJson(t.toJson());

    EXPECT_EQ(t2.id,       1);
    EXPECT_EQ(t2.title,    "Test task");
    EXPECT_EQ(t2.status,   Status::IN_PROGRESS);
    EXPECT_EQ(t2.priority, Priority::HIGH);
    EXPECT_EQ(t2.due_date, "2026-06-01");
}

TEST(TaskTest, StatusConversion) {
    EXPECT_EQ(Task::statusFromString("todo"),        Status::TODO);
    EXPECT_EQ(Task::statusFromString("in_progress"), Status::IN_PROGRESS);
    EXPECT_EQ(Task::statusFromString("done"),        Status::DONE);
    EXPECT_EQ(Task::statusToString(Status::DONE),    "done");
}

TEST(TaskTest, PriorityConversion) {
    EXPECT_EQ(Task::priorityFromString("low"),       Priority::LOW);
    EXPECT_EQ(Task::priorityFromString("high"),      Priority::HIGH);
    EXPECT_EQ(Task::priorityToString(Priority::MEDIUM), "medium");
}
