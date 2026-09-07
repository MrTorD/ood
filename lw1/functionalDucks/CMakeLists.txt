cmake_minimum_required(VERSION 3.15)

set(CURRENT_TASK_NAME "functionalDucks")
project(${CURRENT_TASK_NAME})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
)

list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_executable("${CURRENT_TASK_NAME}_main" ${PROJECT_SOURCES} main.cpp)

target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
