cmake_minimum_required(VERSION 3.10)
project(web_server)
add_executable(web_server main.cpp)
add_library(requests STATIC requests.cpp)
add_library(response STATIC response.cpp)

target_link_libraries(web_server requests response)