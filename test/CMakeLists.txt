cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(testWeb)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)
find_package(fmt REQUIRED)

enable_testing()
include_directories(${PROJECT_SOURCE_DIR}/../inc)

link_directories(${PROJECT_SOURCE_DIR}/../lib)

add_executable(testWeb ${SRC})
target_link_libraries(testWeb PUBLIC
    server logger timer fmt::fmt pthread gtest)
