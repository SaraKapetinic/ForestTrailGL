//
// Created by predrag on 3.3.22..
//

#ifndef CGRAPHICS_DATASTRUCTS_H
#define CGRAPHICS_DATASTRUCTS_H
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
    void LoadFromDisk(std::string path);
    Camera camera;
    glm::vec3 lightColor;
    glm::vec3 lightColor1;
    bool EnableMouseMovement = true;
    float linear = 0.7;
    float quadratic = 0.032;
    bool enableAntialiasing = false;

    ProgramState()
            :camera(glm::vec3(0.0f,4.0f,35.5f)),lightColor(glm::vec3(1.0f,1.0f,1.0f))
            ,lightColor1(glm::vec3(1.0f,1.0f,1.0f)){}

};




#endif //CGRAPHICS_DATASTRUCTS_H
