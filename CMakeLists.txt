cmake_minimum_required(VERSION 3.21)
project(CGraphics)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glad/)
add_executable(CGraphics main.cpp)

target_link_libraries(CGraphics glad)
