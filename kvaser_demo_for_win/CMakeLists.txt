project(kvaser_demo)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB ALL_LIBS "${PROJECT_SOURCE_DIR}/libs/*.lib")
file(GLOB_RECURSE ALL_INCLUDES "${PROJECT_SOURCE_DIR}/include/*.h")

message(STATUS ${ALL_INCLUDES})
link_libraries(${ALL_LIBS})

add_executable(demo main.cpp)

target_link_libraries(demo ${ALL_LIBS})