cmake_minimum_required(VERSION 3.15)

set(CURRENT_TASK_NAME "guiShapes")
project(${CURRENT_TASK_NAME})

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.2
)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        src/*.h
)

list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_executable("${CURRENT_TASK_NAME}_main" ${PROJECT_SOURCES} main.cpp)

target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Model)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Shapes)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Shapes/ConcreteShapes)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Shapes/ShapeProperties)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Controllers)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Views)

target_link_libraries("${CURRENT_TASK_NAME}_main" PRIVATE sfml-graphics sfml-window sfml-system)