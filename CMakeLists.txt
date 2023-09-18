cmake_minimum_required(VERSION 3.16)
project(webapp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# using vcpkg on Windows
# set(VCPKG_ROOT "/mnt/c/src/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE PATH "")
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT})
enable_testing()

# packages
find_package(fmt REQUIRED)

# local variable
set(INCPATH ${PROJECT_SOURCE_DIR}/inc)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory("logger")
add_subdirectory("timer")

# executable
add_executable(webapp)

# target_
target_include_directories(webapp PUBLIC ${INCPATH})
target_sources(webapp PRIVATE main.cpp)
target_link_directories(webapp PRIVATE ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(webapp PRIVATE logger)
