cmake_minimum_required(VERSION 3.21)
project(CGraphics)

set(CMAKE_CXX_STANDARD 20)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)
add_library(STB_IMAGE "model_load/stb_image.cpp")

add_subdirectory(glad/)
add_executable(CGraphics main.cpp model_load/bridge_mesh.cpp model_load/bridge_mesh.h model_load/Shader.cpp model_load/Shader.h model_load/BridgeModel.cpp model_load/BridgeModel.h)

target_link_libraries(CGraphics glad glfw OpenGL::GL pthread X11 Xrandr Xi dl assimp STB_IMAGE)
