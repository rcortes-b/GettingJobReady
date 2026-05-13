# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

CMake 4.x is available via pip (`~/.local/bin/cmake`). Always export the PATH first:

```bash
export PATH="$HOME/.local/bin:$PATH"

# Configure (first time only — downloads nlohmann/json and googletest via FetchContent)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build -j$(nproc)

# Run tests
ctest --test-dir build --output-on-failure

# Run a single test binary directly
./build/tests/taskmanager_tests --gtest_filter="TaskManagerTest.AddTask"
```

The `task` executable lands at `./build/task`. Task data is persisted to `~/.local/share/taskmanager/tasks.json`.

## Architecture

Three distinct layers, each in its own pair of header/source files:

```
src/main.cpp
    └── CLI          (include/CLI.h, src/CLI.cpp)       — parses argv, routes commands, formats output
         └── TaskManager  (include/TaskManager.h, src/TaskManager.cpp)  — CRUD + filtering, holds tasks in memory
              └── Storage      (include/Storage.h, src/Storage.cpp)     — abstract interface + JsonStorage impl
                   └── Task         (include/Task.h, src/Task.cpp)      — plain data struct + JSON serialization
```

**Key design decisions:**
- `Storage` is an abstract class so tests inject `MemoryStorage` without touching the filesystem.
- `TaskManager` persists on every mutation (no explicit flush needed).
- `Task` owns its own `toJson`/`fromJson` — nlohmann/json is included in `Task.h` and flows transitively.
- C++20 (`std::optional`, `std::filesystem`, `std::string::rfind` for prefix checks).

## Commands

```
task add <title> [--desc <text>] [--priority low|medium|high] [--due YYYY-MM-DD]
task list        [--status todo|in_progress|done] [--priority low|medium|high]
task done <id>
task show <id>
task edit <id>   [--title <text>] [--status todo|in_progress|done]
task delete <id>
```
