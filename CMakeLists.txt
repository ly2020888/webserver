cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILECOMMANDS ON)

project(webapp)
enable_testing()

find_package(fmt REQUIRED)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp)

set(INCPATH ${PROJECT_SOURCE_DIR}/inc)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory("logger")
add_subdirectory("timer")

include_directories(${INCPATH})

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(webapp ${SRC})
target_link_libraries(webapp PRIVATE logger)
