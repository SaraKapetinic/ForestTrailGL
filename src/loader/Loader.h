//
// Created by predrag on 4.4.22..
//

#ifndef FORESTTRAILGL_LOADER_H
#define FORESTTRAILGL_LOADER_H
#include "Shader.h"
#include "Model.h"
#include "SkyBox.h"
#include <filesystem>

void loadModels(std::unordered_map<std::string, Model>& models){
    std::string bridgePath = std::filesystem::path("../resources/models/bridge.obj");
    std::string streetLampPath = std::filesystem::path("../resources/models/StreetLamp/StreetLamp.obj");
    std::string lightBulbPath = std::filesystem::path("../resources/models/LightBulb/lightBulb.obj");

    Model bridgeModel = Model(bridgePath.c_str());
    Model streetLampModel(streetLampPath.c_str());
    Model lightBulbModel(lightBulbPath.c_str());

    models.emplace("bridge", bridgeModel);
    models.emplace("lamp", streetLampModel);
    models.emplace("lightBulb", lightBulbModel);
}

void loadShaders(std::unordered_map<std::string, Shader>& shaders){
    Shader mainShader("../resources/shaders/model.vs", "../resources/shaders/model.fs");
    Shader terrainShader("../resources/shaders/model.vs", "../resources/shaders/terrain.fs");
    Shader skyBoxShader("../resources/shaders/skybox.vs", "../resources/shaders/skybox.fs");
    Shader waterShader("../resources/shaders/model.vs", "../resources/shaders/water.fs");
    Shader instanceShader("../resources/shaders/model_instances.vs", "../resources/shaders/model.fs");

    Shader depthShader("../resources/shaders/depth.vs", "../resources/shaders/depth.fs", "../resources/shaders/depth.gs");
    Shader hdr("../resources/shaders/hdr.vs", "../resources/shaders/hdr.fs");


    shaders.emplace("main", mainShader);
    shaders.emplace("terrain", terrainShader);
    shaders.emplace("skybox", skyBoxShader);
    shaders.emplace("water", waterShader);
    shaders.emplace("instance", instanceShader);

    shaders.emplace("depth", depthShader);

    shaders.emplace("hdr", hdr);

}

void loadInstancedModels(std::unordered_map<std::string, InstancedModel>& models){
    std::string treeModelPath = std::filesystem::path("../resources/models/Tree/tree.obj");
    InstancedModel treeModel(treeModelPath.c_str());
    models.emplace("tree", treeModel);
}

TerrainModel loadTerrain(float& SIZE){
    Terrain t(0,0,25);
    SIZE = t.getSize();
    return t.generateTerrain();
}

WaterModel loadWater(float& SIZE) {
    Water w(0, 0, 25);
    SIZE = w.getSize();
    return w.generateWater();
}

SkyBox loadSkybox() {
    std::vector<std::string> skyboxFaces = {
            "../resources/skybox/daylight/right.bmp",
            "../resources/skybox/daylight/left.bmp",
            "../resources/skybox/daylight/top.bmp",
            "../resources/skybox/daylight/bottom.bmp",
            "../resources/skybox/daylight/back.bmp",
            "../resources/skybox/daylight/front.bmp"
    };
    std::vector<std::string> nightSkyboxFaces = {
            "../resources/skybox/nightsky/xneg.jpg",
            "../resources/skybox/nightsky/xpos.jpg",
            "../resources/skybox/nightsky/zpos.jpg",
            "../resources/skybox/nightsky/zneg.jpg",
            "../resources/skybox/nightsky/yneg.jpg",
            "../resources/skybox/nightsky/ypos.jpg"

    };

    return SkyBox(skyboxFaces, nightSkyboxFaces);
}


#endif //FORESTTRAILGL_LOADER_H
