#include <gtest/gtest.h>
#include "TaskManager.h"
#include "Storage.h"

class MemoryStorage : public Storage {
public:
    std::vector<Task> data;
    std::vector<Task> load() override { return data; }
    void save(const std::vector<Task>& tasks) override { data = tasks; }
};

class TaskManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto storage = std::make_unique<MemoryStorage>();
        storagePtr   = storage.get();
        manager      = std::make_unique<TaskManager>(std::move(storage));
    }

    MemoryStorage*            storagePtr;
    std::unique_ptr<TaskManager> manager;
};

TEST_F(TaskManagerTest, AddTask) {
    Task t = manager->add("Buy milk");
    EXPECT_EQ(t.id,     1);
    EXPECT_EQ(t.title,  "Buy milk");
    EXPECT_EQ(t.status, Status::TODO);
}

TEST_F(TaskManagerTest, AddMultiple) {
    manager->add("Task 1");
    Task t2 = manager->add("Task 2");
    EXPECT_EQ(t2.id, 2);
    EXPECT_EQ(manager->listAll().size(), 2u);
}

TEST_F(TaskManagerTest, DeleteTask) {
    manager->add("Task 1");
    EXPECT_TRUE(manager->remove(1));
    EXPECT_TRUE(manager->listAll().empty());
}

TEST_F(TaskManagerTest, DeleteNonExistent) {
    EXPECT_FALSE(manager->remove(999));
}

TEST_F(TaskManagerTest, UpdateStatus) {
    manager->add("Task 1");
    EXPECT_TRUE(manager->updateStatus(1, Status::DONE));
    auto t = manager->get(1);
    ASSERT_TRUE(t.has_value());
    EXPECT_EQ(t->status, Status::DONE);
}

TEST_F(TaskManagerTest, FilterByStatus) {
    manager->add("Task 1");
    manager->add("Task 2");
    manager->updateStatus(1, Status::DONE);

    auto done = manager->filter(Status::DONE, std::nullopt);
    EXPECT_EQ(done.size(), 1u);
    EXPECT_EQ(done[0].title, "Task 1");
}

TEST_F(TaskManagerTest, Persistence) {
    manager->add("Persisted task");
    EXPECT_EQ(storagePtr->data.size(), 1u);
}
