cmake_minimum_required(VERSION 3.15)

set(CURRENT_TASK_NAME "eventShapes")
project(${CURRENT_TASK_NAME})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
)

list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_executable("${CURRENT_TASK_NAME}_main" ${PROJECT_SOURCES} main.cpp)
add_executable("${CURRENT_TASK_NAME}_tests" ${PROJECT_SOURCES} tests.cpp)

target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Canvas)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Errors)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/PictureCommands)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/PictureCommands/ConcreteCommands)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/PictureCommands/ConcreteCommands)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Shapes/ShapeGeometries)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Shapes/ShapeProperties)
target_include_directories(${CURRENT_TASK_NAME}_main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Events)

target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Canvas)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Errors)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/PictureCommands)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/PictureCommands/ConcreteCommands)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/PictureCommands/ConcreteCommands)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Shapes/ShapeGeometries)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lw1/shapes/src/Shapes/ShapeProperties)
target_include_directories(${CURRENT_TASK_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Events)


target_link_libraries("${CURRENT_TASK_NAME}_tests" PRIVATE GTest::gtest GTest::gtest_main GTest::gmock )

add_test(NAME "${CURRENT_LAB_NAME}.${CURRENT_TASK_NAME}" COMMAND "${CURRENT_TASK_NAME}_tests")
