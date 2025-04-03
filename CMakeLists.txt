cmake_minimum_required(VERSION 3.10)
project(web_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_executable(web_server main.cpp)
add_library(requests STATIC requests.cpp)

target_link_libraries(web_server requests)