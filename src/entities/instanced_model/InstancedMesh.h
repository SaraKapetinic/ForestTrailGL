//
// Created by predrag on 30.3.22..
//

#ifndef FORESTTRAILGL_INSTANCEDMESH_H
#define FORESTTRAILGL_INSTANCEDMESH_H
#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>
#include "DataStructs.h"

class InstancedMesh {
public:

    InstancedMesh(std::vector<Vertex> v, std::vector<unsigned> i, std::vector<Texture> t);
    void Draw(Shader& shader, int n);
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<Texture> textures;
    unsigned VAO, VBO, EBO;
    void setupInstancedMesh();
};


#endif //FORESTTRAILGL_INSTANCEDMESH_H
