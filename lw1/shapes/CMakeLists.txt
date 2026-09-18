cmake_minimum_required(VERSION 3.15)

set(CURRENT_TASK_NAME "shapes")
project(${CURRENT_TASK_NAME})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
)

list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_executable("${CURRENT_TASK_NAME}_main" ${PROJECT_SOURCES} main.cpp)
add_executable("${CURRENT_TASK_NAME}_tests" ${PROJECT_SOURCES} tests.cpp)

target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Canvas)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Errors)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/PictureCommands)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/PictureCommands/ConcreteCommands)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Shapes)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Shapes/ShapeGeometries)

target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Canvas)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Errors)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/PictureCommands)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/PictureCommands/ConcreteCommands)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Shapes)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Shapes/ShapeGeometries)

target_link_libraries("${CURRENT_TASK_NAME}_tests" PRIVATE GTest::gtest GTest::gtest_main GTest::gmock )

add_test(NAME "${CURRENT_LAB_NAME}.${CURRENT_TASK_NAME}" COMMAND "${CURRENT_TASK_NAME}_tests")
