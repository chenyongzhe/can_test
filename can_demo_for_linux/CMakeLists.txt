project(can_demo)
cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 11)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

file(GLOB LIBS "${PROJECT_SOURCE_DIR}/libs/*")
file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*")

find_package(Threads REQUIRED)

message(STATUS ${LIBS})
message(STATUS ${SRCS})

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/libs)

add_executable(demo ${SRCS})
target_link_libraries(demo ${LIBS})
