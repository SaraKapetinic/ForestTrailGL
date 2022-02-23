cmake_minimum_required(VERSION 3.16.3)
project(CGraphics)

set(CMAKE_CXX_STANDARD 20)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(glad/)
add_executable(CGraphics main.cpp)

target_link_libraries(CGraphics glad glfw OpenGL::GL pthread X11 Xrandr Xi dl)
