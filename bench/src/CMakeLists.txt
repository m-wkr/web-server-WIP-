cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(server-bench)
add_executable(server-bench main.cpp)


include_directories(../../server_core)
file(GLOB SERVER_CORE_SOURCES "../../server_core/*.cpp")
add_library(server_core_lib STATIC ${SERVER_CORE_SOURCES})


find_package(benchmark REQUIRED PATHS ../benchmark/build)


target_link_libraries(server-bench benchmark::benchmark server_core_lib)

target_include_directories(benchmark::benchmark INTERFACE ../benchmark/include)
target_include_directories(server-bench PRIVATE "../..server_core")