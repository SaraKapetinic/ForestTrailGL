//
// Created by predrag on 1.3.22..
//

#ifndef CGRAPHICS_TERRAINMODEL_H
#define CGRAPHICS_TERRAINMODEL_H
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h>
#include <string>
#include "../model/Shader.h"
#include "../tools/DataStructs.h"



class TerrainModel {
public:
    TerrainModel(std::vector<Vertex>& Vertices, std::vector<unsigned>& Indices){
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, Vertices.size()*sizeof(Vertex), &Vertices[0],GL_STATIC_DRAW);
        glGenBuffers(1,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned), &Indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex) , (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));

        glBindVertexArray(0);

        numOfIndices = Indices.size();

        loadTerrainTexture("../resources/TERRAINTEX.JPG");
    }

    void Draw(Shader& shader);

private:
    unsigned VAO, VBO, EBO;
    Texture terrainTex;
    int numOfIndices;

    void loadTerrainTexture(const char *path);
};


#endif //CGRAPHICS_TERRAINMODEL_H
