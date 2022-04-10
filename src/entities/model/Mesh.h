//
// Created by predrag on 18.2.22..
//

#ifndef FORESTTRAILGL_MESH_H
#define FORESTTRAILGL_MESH_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>
#include "DataStructs.h"

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

private:
    unsigned VAO, VBO, EBO;
    void setupMesh();
};


#endif //FORESTTRAILGL_MESH_H
