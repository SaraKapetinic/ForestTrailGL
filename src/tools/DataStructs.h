//
// Created by predrag on 3.3.22..
//

#ifndef FORESTTRAILGL_DATASTRUCTS_H
#define FORESTTRAILGL_DATASTRUCTS_H
#include <string>
#include <glm/glm.hpp>
#include "Camera.h"


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned id;
    std::string type;
    std::string path;
};


struct ProgramState{
    bool ImguiEnable= false;
    Camera camera = Camera(glm::vec3(0.0f, 4.0f, 35.5f));
    glm::vec3 lightColor[3];
    glm::vec3 lightPosition[3];
    bool EnableMouseMovement = true;
    float linear = 0.7;
    float quadratic = 0.032;
    bool enableAntialiasing = true;
    unsigned SCR_WIDTH = 800;
    unsigned SCR_HEIGHT = 600;
    bool shadows = false;
    bool isDay = true;
    float terrainSize;
    float waterSize;
    float skyboxOpacity=0;
    bool skyBoxChange = false;
    int lightIndex = 0;

};




#endif //FORESTTRAILGL_DATASTRUCTS_H
