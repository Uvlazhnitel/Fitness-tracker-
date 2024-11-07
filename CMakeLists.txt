cmake_minimum_required(VERSION 3.10)
project(fitness_tracker)

set(CMAKE_CXX_STANDARD 11)

# Добавляем исходный файл
add_executable(fitness_tracker main.cpp)

# Подключение wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
if (wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(fitness_tracker ${wxWidgets_LIBRARIES})
else()
    message(FATAL_ERROR "wxWidgets not found!")
endif()

# Подключение SQLite3
find_package(SQLite3 REQUIRED)
if (SQLite3_FOUND)
    target_include_directories(fitness_tracker PRIVATE ${SQLite3_INCLUDE_DIRS})
    target_link_libraries(fitness_tracker ${SQLite3_LIBRARIES})
else()
    message(FATAL_ERROR "SQLite3 library not found!")
endif()