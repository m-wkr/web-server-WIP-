add_library(
    server_core
    mimeTypes.hpp
    requests.cpp
    requests.hpp
    response.hpp
    server.hpp
    socketWrapper.hpp
)

target_include_directories(server_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})