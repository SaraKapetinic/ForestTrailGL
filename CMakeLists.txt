cmake_minimum_required(VERSION 3.16.3)
project(CGraphics)

set(CMAKE_CXX_STANDARD 20)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)
add_library(STB_IMAGE "model/stb_image.cpp")

add_subdirectory(glad/)
add_subdirectory(imgui/)
add_executable(CGraphics main.cpp model/Mesh.cpp model/Mesh.h model/Shader.cpp model/Shader.h model/Model.cpp model/Model.h model/Camera.h terrain/Terrain.cpp terrain/Terrain.h terrain/TerrainModel.cpp terrain/TerrainModel.h tools/DataStructs.h skybox/SkyBox.cpp skybox/SkyBox.h)

target_link_libraries(CGraphics glad glfw OpenGL::GL pthread X11 Xrandr Xi dl assimp STB_IMAGE imgui)
