cmake_minimum_required(VERSION 3.6)
project(antouch_server)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/main.c src/XlibWrapper.h src/XlibWrapper.c src/AntouchServer.h src/AntouchServer.c src/ProtoAtci.h src/ProtoAtci.c)
add_executable(antouch_server ${SOURCE_FILES})
target_link_libraries(antouch_server ev X11 Xtst xdo)