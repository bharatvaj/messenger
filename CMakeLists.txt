cmake_minimum_required(VERSION 3.8)
project(messenger CXX)

find_package(clog CONFIG REQUIRED)
find_package(Threads REQUIRED)

set(BUFFER_SIZE 256)
set(DEFAULT_PORT 8400)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h"
)

add_executable(messenger messenger.cpp)
target_compile_features(messenger PRIVATE cxx_std_11)

if(EMSCRIPTEN)
	set(CMAKE_CXX_STANDARD 11)
	set_target_properties(messenger PROPERTIES 
		LINK_FLAGS "-g4 -s WASM=1 -s SOCKET_DEBUG=1"
	)
endif()

target_include_directories(messenger PRIVATE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
	$<INSTALL_INTERFACE:include>
)

target_link_libraries(messenger PRIVATE clog::clog)
target_link_libraries(messenger PRIVATE ${CMAKE_THREAD_LIBS_INIT})
