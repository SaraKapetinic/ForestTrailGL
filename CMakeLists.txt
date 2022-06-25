cmake_minimum_required(VERSION 3.16.3)
project(ForestTrailGL)

set(CMAKE_CXX_STANDARD 20)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
add_library(STB_IMAGE "external/stb_image.cpp")

add_subdirectory(external/glad/)
add_subdirectory(external/imgui/)

include_directories(external src src/entities/model src/entities/terrain src/entities/skybox src/tools src/gui src/entities/water src/entities/instanced_model src/renderer src/loader)


add_executable(ForestTrailGL src/main.cpp src/entities/model/Mesh.cpp src/entities/model/Mesh.h src/Shader.cpp src/Shader.h src/entities/model/Model.cpp src/entities/model/Model.h src/Camera.h src/entities/terrain/Terrain.cpp src/entities/terrain/Terrain.h src/entities/terrain/TerrainModel.cpp src/entities/terrain/TerrainModel.h src/tools/DataStructs.h src/entities/skybox/SkyBox.cpp src/entities/skybox/SkyBox.h src/gui/GUI.h src/entities/water/Water.cpp src/entities/water/Water.h src/entities/water/WaterModel.cpp src/entities/water/WaterModel.h src/entities/instanced_model/InstancedMesh.cpp src/entities/instanced_model/InstancedMesh.h src/entities/instanced_model/InstancedModel.cpp src/entities/instanced_model/InstancedModel.h src/tools/TreeTranslation.h src/renderer/Renderer.cpp src/renderer/Renderer.h src/loader/Loader.h)

target_link_libraries(ForestTrailGL glad glfw OpenGL::GL pthread X11 Xrandr Xi dl assimp STB_IMAGE imgui)
