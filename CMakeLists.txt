cmake_minimum_required(VERSION 2.8)
project(messenger)
find_package(Threads REQUIRED)

add_definitions(-DENABLE_LOG)

set(BUFFER_SIZE 256)
set(DEFAULT_PORT 8400)

configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_SOURCE_DIR}/config.h"
)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-fPIC -Wall")

include_directories(${PROJECT_SOURCE_DIR})

add_executable(messenger messenger.cpp)
target_link_libraries(messenger ${CMAKE_THREAD_LIBS_INIT})
