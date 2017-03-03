cmake_minimum_required(VERSION 3.6)
project(antouch_server)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c xlib_wrapper.h xlib_wrapper.c antouch_server.h antouch_server.c broadcast_connection.h broadcast_connection.c)
add_executable(antouch_server ${SOURCE_FILES})
target_link_libraries(antouch_server ev X11 Xtst)