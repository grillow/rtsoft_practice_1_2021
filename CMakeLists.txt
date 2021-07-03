cmake_minimum_required(VERSION 3.19)
project(task1)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic")

include_directories(include)
file(GLOB SOURCE src/*.cpp)
add_executable(${PROJECT_NAME} SOURCES)

