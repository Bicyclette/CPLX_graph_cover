cmake_minimum_required(VERSION 3.16)

project(graph_cover)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT ${CMAKE_BINARY_DIR})

include_directories(include)

set(SRCS
	src/main.cpp
	src/graph.cpp)

set(HEADERS
	include/graph.hpp)

add_executable(${PROJECT_NAME} ${SRCS} ${HEADERS})
