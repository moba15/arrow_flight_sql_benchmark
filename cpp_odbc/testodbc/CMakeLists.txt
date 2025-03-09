cmake_minimum_required(VERSION 2.20)
project(testodbc)

set(CMAKE_CXX_STANDARD 20)
find_package(ODBC REQUIRED)

# Include ODBC headers
include_directories(${ODBC_INCLUDE_DIR})

# Set your C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add executable


# Link with ODBC library


add_executable(testodbc main.cpp)
target_link_libraries(testodbc ${ODBC_LIBRARIES})
