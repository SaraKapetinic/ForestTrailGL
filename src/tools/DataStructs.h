//
// Created by predrag on 3.3.22..
//

#ifndef CGRAPHICS_DATASTRUCTS_H
#define CGRAPHICS_DATASTRUCTS_H
#include <string>
#include <glm/glm.hpp>

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


#endif //CGRAPHICS_DATASTRUCTS_H
