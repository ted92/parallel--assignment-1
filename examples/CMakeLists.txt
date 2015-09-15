cmake_minimum_required(VERSION 3.3)
project(examples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    mpi-hello.c
    mpi-hello2.c
	first-mpi.c)

add_executable(examples ${SOURCE_FILES})