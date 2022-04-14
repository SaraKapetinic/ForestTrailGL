//
// Created by predrag on 4.4.22..
//

#ifndef FORESTTRAILGL_RENDERER_H
#define FORESTTRAILGL_RENDERER_H
#include "Model.h"
#include "TerrainModel.h"
#include "WaterModel.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "InstancedModel.h"
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer(std::unordered_map<std::string, Model> &m, std::unordered_map<std::string, InstancedModel>& im, TerrainModel& t, WaterModel& w, SkyBox& sk,  ProgramState &p, std::unordered_map<std::string, Shader> &s)
            : models(m), ps(p), shaders(s), waterModel(w), terrainModel(t), instancedModels(im), skyBox(sk){

        initializeDepthBuffer();
        prepareHDR();
        prepareAntialiasing();
    };
    void setMatrices(glm::mat4 v, glm::mat4 p){
        view = v;
        projection = p;
    }
    void renderTerrain(Shader &shader);
    void renderSkybox(Shader &shader);
    void renderModels(Shader &shader);
    void renderScene();
    void renderWater(Shader& shader);
    void renderInstancedModel(Shader& shader);
    void prepareHDR();
    void renderQuad();
private:
    std::unordered_map<std::string , Model>& models;
    std::unordered_map<std::string, Shader>& shaders;
    std::unordered_map<std::string, InstancedModel>& instancedModels;
    unsigned depthMapFBO;
    unsigned depthCubemap;
    const unsigned SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    TerrainModel& terrainModel;
    unsigned hdrFBO;
    unsigned sampleTex;
    unsigned colorBuffer;
    unsigned multisampleFBO;
    WaterModel& waterModel;
    SkyBox& skyBox;
    glm::mat4 view, projection;
    ProgramState& ps;
    void initializeShader(Shader &shader);
    void initializeDepthBuffer();
    void renderToDepthBuffer();
    void prepareAntialiasing();
};


#endif //FORESTTRAILGL_RENDERER_H
