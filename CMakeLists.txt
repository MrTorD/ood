cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 23)

set(ROOT_PROJECT_NAME "OOD-2026")
project(${ROOT_PROJECT_NAME})

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

# add_subdirectory(lw1)
add_subdirectory(lw2)