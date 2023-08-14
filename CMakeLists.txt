cmake_minimum_required(VERSION 3.10)

project(png-base64)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Use pkg-config to get the flags for libwebp
find_package(PkgConfig REQUIRED)
pkg_check_modules(WEBP REQUIRED libwebp)

# Source files
add_executable(png-base64 demo/main.cpp)

# Link against required libraries
find_package(PNG REQUIRED)
target_include_directories(png-base64 PRIVATE ${WEBP_INCLUDE_DIRS})
target_link_libraries(png-base64 PNG::PNG ${WEBP_LIBRARIES})
