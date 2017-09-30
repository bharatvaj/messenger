cmake_minimum_required(VERSION 2.8)
project(messenger)
find_package(Threads REQUIRED)


set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-fPIC -Wall")

include_directories(${PROJECT_SOURCE_DIR})

add_executable(messenger messenger.cpp)
target_link_libraries(messenger ${CMAKE_THREAD_LIBS_INIT})
