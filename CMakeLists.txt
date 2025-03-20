cmake_minimum_required(VERSION 3.10)
project(web_server)
add_executable(web_server main.cpp)
add_library(requests STATIC requests.cpp)


target_link_libraries(web_server requests)
set_property(TARGET requests PROPERTY CXX_STANDARD 20)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED True)