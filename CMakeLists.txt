cmake_minimum_required(VERSION 3.10)
project(messenger CXX)

find_package(Threads REQUIRED)
find_package(clog REQUIRED)

set(BUFFER_SIZE 256)
set(DEFAULT_PORT 8400)

configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_SOURCE_DIR}/config.h"
)

add_executable(messenger messenger.cpp)

target_include_directories(messenger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(messenger PRIVATE clog::clog)
target_link_libraries(messenger PRIVATE ${CMAKE_THREAD_LIBS_INIT})
