cmake_minimum_required(VERSION 3.3)

file(GLOB IMGUI_HEADERS "includes/*.h")
file(GLOB IMGUI_SOURCES "src/*.cpp")

add_library(imgui ${IMGUI_HEADERS} ${IMGUI_SOURCES})
target_link_libraries(imgui glad)
target_include_directories(imgui PUBLIC includes)
target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLAD)