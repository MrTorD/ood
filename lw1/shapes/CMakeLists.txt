cmake_minimum_required(VERSION 3.15)

set(CURRENT_TASK_NAME "shapes")
project(${CURRENT_TASK_NAME})

# file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
#         "*.cpp"
#         "*.h"
# )

HEADER_DIRECTORIES(header_dir_list)

add_executable("${CURRENT_TASK_NAME}_main" ${header_dir_list} main.cpp)
add_executable("${CURRENT_TASK_NAME}_tests" ${header_dir_list} tests.cpp)

target_include_directories(
    ${CURRENT_TASK_NAME}_main
    PRIVATE
    ${header_dir_list}
)

target_include_directories(
    ${CURRENT_TASK_NAME}_tests
    PRIVATE
    ${header_dir_list}
)

target_link_libraries("${CURRENT_TASK_NAME}_tests" PRIVATE GTest::gtest GTest::gtest_main GTest::gmock )

add_test(NAME "${CURRENT_LAB_NAME}.${CURRENT_TASK_NAME}" COMMAND "${CURRENT_TASK_NAME}_tests")
