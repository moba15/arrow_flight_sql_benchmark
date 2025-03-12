cmake_minimum_required(VERSION 3.5)
project(testodbc)

set(CMAKE_CXX_STANDARD 20)
find_package(ODBC REQUIRED)

# Include ODBC headers
include_directories(${ODBC_INCLUDE_DIR})

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,/usr/local/lib")
# Set your C++ standard
set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/include)

# Add executable


# Link with ODBC library


add_executable(testodbc main.cpp)
target_link_libraries(testodbc  PRIVATE odbccpp )
