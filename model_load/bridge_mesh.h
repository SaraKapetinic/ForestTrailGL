//
// Created by predrag on 18.2.22..
//

#ifndef CGRAPHICS_BRIDGE_MESH_H
#define CGRAPHICS_BRIDGE_MESH_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>
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

class bridge_mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        std::vector<Texture> textures;

        bridge_mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

private:
    unsigned VAO, VBO, EBO;
    void setupMesh();
};


#endif //CGRAPHICS_BRIDGE_MESH_H
