cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
cmake_policy(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

project(boost_beast_playground)

find_package(Boost REQUIRED COMPONENTS asio beast)

file(GLOB headers CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(client STATIC ${headers} ${sources})
target_precompile_headers(client PUBLIC ${PROJECT_SOURCE_DIR}/stdafx.hpp)
target_include_directories(client PUBLIC src)
target_link_libraries(client PUBLIC Boost::asio Boost::beast)
target_compile_definitions(client PUBLIC BOOST_USE_WINDOWS_H)

add_executable(keepalive
	${PROJECT_SOURCE_DIR}/examples/keepalive.cpp
)
target_precompile_headers(keepalive REUSE_FROM client)
target_link_libraries(keepalive PRIVATE client)