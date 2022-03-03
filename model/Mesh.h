//
// Created by predrag on 18.2.22..
//

#ifndef CGRAPHICS_MESH_H
#define CGRAPHICS_MESH_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>
#include "../tools/DataStructs.h"

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


#endif //CGRAPHICS_MESH_H
