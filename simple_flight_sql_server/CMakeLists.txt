cmake_minimum_required(VERSION 3.28)
project(arrow_flight_sql_simple)

set(CMAKE_CXX_STANDARD 17)

add_executable(arrow_flight_sql_simple main.cpp)

add_subdirectory(tools/server)