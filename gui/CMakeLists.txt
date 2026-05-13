set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(task-gui
    main_gui.cpp
    MainWindow.cpp
    TaskTableModel.cpp
    AddTaskDialog.cpp
)

target_include_directories(task-gui PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(task-gui PRIVATE tasklib Qt6::Widgets)
