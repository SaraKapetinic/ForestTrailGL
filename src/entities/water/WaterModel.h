//
// Created by predrag on 28.3.22..
//

#ifndef CGRAPHICS_WATERMODEL_H
#define CGRAPHICS_WATERMODEL_H
#include "DataStructs.h"
#include "Shader.h"

class WaterModel {
public:
    WaterModel(std::vector<Vertex>& Vertices, std::vector<unsigned>& Indices){
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

        loadWaterTexture("../resources/water/watertext.jpg");


    }

    void Draw(Shader& shader);

private:
    unsigned VAO, VBO, EBO;
    Texture waterTex;
    int numOfIndices;

    void loadWaterTexture(std::string filename);

};


#endif //CGRAPHICS_WATERMODEL_H
