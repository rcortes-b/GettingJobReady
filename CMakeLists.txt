cmake_minimum_required(VERSION 3.20)
project(taskmanager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Shared library target (reused by executable and tests)
add_library(tasklib
    src/Task.cpp
    src/Storage.cpp
    src/TaskManager.cpp
    src/CLI.cpp
)
target_include_directories(tasklib PUBLIC include)
target_link_libraries(tasklib PUBLIC nlohmann_json::nlohmann_json)

# Main executable
add_executable(task src/main.cpp)
target_link_libraries(task PRIVATE tasklib)

# Tests
enable_testing()
add_subdirectory(tests)

# GUI (optional — only built if Qt6 is available)
find_package(Qt6 QUIET COMPONENTS Widgets)
if(Qt6_FOUND)
    message(STATUS "Qt6 found — building task-gui")
    add_subdirectory(gui)
else()
    message(STATUS "Qt6 not found — skipping task-gui")
endif()
