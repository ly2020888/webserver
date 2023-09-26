cmake_minimum_required(VERSION 3.16)
project(webapp)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# using vcpkg on Windows
# set(VCPKG_ROOT "/mnt/c/src/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
enable_testing()

# packages
find_package(fmt REQUIRED)

# local variable
set(INCPATH ${PROJECT_SOURCE_DIR}/inc)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory("logger")
add_subdirectory("timer")
add_subdirectory("server")
add_subdirectory("util")
add_subdirectory("test")

# executable
add_executable(webapp)

# target_
target_include_directories(webapp PUBLIC ${INCPATH} ${INCJSON})
target_sources(webapp PRIVATE main.cpp)
target_link_directories(webapp PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(webapp PRIVATE server logger util)
