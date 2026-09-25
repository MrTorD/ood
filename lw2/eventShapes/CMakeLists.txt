cmake_minimum_required(VERSION 3.15)

set(CURRENT_TASK_NAME "eventShapes")
project(${CURRENT_TASK_NAME})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
        src/*.h
)

FetchContent_Declare(
  signals2
  GIT_REPOSITORY https://github.com/boostorg/signals2.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  assert
  GIT_REPOSITORY https://github.com/boostorg/assert.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  bind
  GIT_REPOSITORY https://github.com/boostorg/bind.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  config
  GIT_REPOSITORY https://github.com/boostorg/config.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  core
  GIT_REPOSITORY https://github.com/boostorg/core.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  function
  GIT_REPOSITORY https://github.com/boostorg/function.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  iterator
  GIT_REPOSITORY https://github.com/boostorg/iterator.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  mpl
  GIT_REPOSITORY https://github.com/boostorg/mpl.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  multi_index
  GIT_REPOSITORY https://github.com/boostorg/multi_index.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  optional
  GIT_REPOSITORY https://github.com/boostorg/optional.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  parameter
  GIT_REPOSITORY https://github.com/boostorg/parameter.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  predef
  GIT_REPOSITORY https://github.com/boostorg/predef.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  preprocessor
  GIT_REPOSITORY https://github.com/boostorg/preprocessor.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  smart_ptr
  GIT_REPOSITORY https://github.com/boostorg/smart_ptr.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  throw_exception
  GIT_REPOSITORY https://github.com/boostorg/throw_exception.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  tuple
  GIT_REPOSITORY https://github.com/boostorg/tuple.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  type_traits
  GIT_REPOSITORY https://github.com/boostorg/type_traits.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  variant
  GIT_REPOSITORY https://github.com/boostorg/variant.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  concept_check
  GIT_REPOSITORY https://github.com/boostorg/concept_check.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  utility
  GIT_REPOSITORY https://github.com/boostorg/utility.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  fusion
  GIT_REPOSITORY https://github.com/boostorg/fusion.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  move
  GIT_REPOSITORY https://github.com/boostorg/move.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  container_hash
  GIT_REPOSITORY https://github.com/boostorg/container_hash.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  function_types
  GIT_REPOSITORY https://github.com/boostorg/function_types.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  describe
  GIT_REPOSITORY https://github.com/boostorg/describe.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  mp11
  GIT_REPOSITORY https://github.com/boostorg/mp11.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  io
  GIT_REPOSITORY https://github.com/boostorg/io.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  detail
  GIT_REPOSITORY https://github.com/boostorg/detail.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  integer
  GIT_REPOSITORY https://github.com/boostorg/integer.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  typeof
  GIT_REPOSITORY https://github.com/boostorg/typeof.git
  GIT_TAG boost-1.92.0
)
FetchContent_Declare(
  type_index
  GIT_REPOSITORY https://github.com/boostorg/type_index.git
  GIT_TAG boost-1.92.0
)

FetchContent_MakeAvailable(signals2)
FetchContent_MakeAvailable(assert)
FetchContent_MakeAvailable(bind)
FetchContent_MakeAvailable(config)
FetchContent_MakeAvailable(core)
FetchContent_MakeAvailable(function)
FetchContent_MakeAvailable(iterator)
FetchContent_MakeAvailable(mpl)
FetchContent_MakeAvailable(multi_index)
FetchContent_MakeAvailable(optional)
FetchContent_MakeAvailable(parameter)
FetchContent_MakeAvailable(predef)
FetchContent_MakeAvailable(preprocessor)
FetchContent_MakeAvailable(smart_ptr)
FetchContent_MakeAvailable(throw_exception)
FetchContent_MakeAvailable(tuple)
FetchContent_MakeAvailable(type_traits)
FetchContent_MakeAvailable(variant)
FetchContent_MakeAvailable(concept_check)
FetchContent_MakeAvailable(utility)
FetchContent_MakeAvailable(fusion)
FetchContent_MakeAvailable(move)
FetchContent_MakeAvailable(container_hash)
FetchContent_MakeAvailable(function_types)
FetchContent_MakeAvailable(describe)
FetchContent_MakeAvailable(mp11)
FetchContent_MakeAvailable(io)
FetchContent_MakeAvailable(detail)
FetchContent_MakeAvailable(integer)
FetchContent_MakeAvailable(typeof)
FetchContent_MakeAvailable(type_index)

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


target_link_libraries("${CURRENT_TASK_NAME}_main" PRIVATE Boost::signals2)

target_link_libraries("${CURRENT_TASK_NAME}_tests" PRIVATE GTest::gtest GTest::gtest_main GTest::gmock Boost::signals2 )

add_test(NAME "${CURRENT_LAB_NAME}.${CURRENT_TASK_NAME}" COMMAND "${CURRENT_TASK_NAME}_tests")
