add_executable(taskmanager_tests
    test_task.cpp
    test_task_manager.cpp
)
target_link_libraries(taskmanager_tests PRIVATE tasklib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(taskmanager_tests)
