//
// Created by predrag on 27.2.22..
//

#include "Terrain.h"
#include <iostream>
#include "stb_image.h"

TerrainModel Terrain::generateTerrain() {

    loadHeightMap("../resources/terrain/heightMap.png");
    if(heightMap == nullptr) {
        std::cout << "Error loading heightmap!" << std::endl;
        exit(EXIT_FAILURE);
    }
    VERTEX_COUNT = heightMapHeight;
    indices.resize(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));

    for(int i=0;i<VERTEX_COUNT;i++){
        for(int j=0;j<VERTEX_COUNT;j++){
            float x, z;

            x = (float)j/((float)VERTEX_COUNT - 1);
            z = (float)i/((float)VERTEX_COUNT - 1);

            glm::vec3 Position( x * SIZE, getHeight(j,i),  z * SIZE);
            glm::vec3 Normal = getNormal(j,i);
            glm::vec2 TextureCords(x, z);
            Vertex v;
            v.Position = Position;
            v.Normal = Normal;
            v.TexCoords = TextureCords * scale;
            vertices.push_back(v);
        }
    }

    int pointer = 0;
    for(int gz=0;gz<VERTEX_COUNT-1;gz++){
        for(int gx=0;gx<VERTEX_COUNT-1;gx++){
            int topLeft = (gz*VERTEX_COUNT)+gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
            int bottomRight = bottomLeft + 1;
            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }

    stbi_image_free(heightMap);
    return TerrainModel(vertices, indices);

}

void Terrain::loadHeightMap(std::string path) {
    int nrChannels;
    heightMap = stbi_load(path.c_str(),&heightMapWidth, &heightMapHeight, &nrChannels, 0);
    if(!heightMap) {
        stbi_image_free(heightMap);
        heightMap = nullptr;
    }
}

float Terrain::getSize() const {
    return SIZE;
}

float Terrain::getHeight(int xpos, int ypos) {

    if(xpos < 0 || xpos>=heightMapHeight || ypos < 0 || ypos>=heightMapHeight)
        return 0;
    float r = heightMap[4 * ( ypos * heightMapWidth + xpos)];
    r/=255.0;
    r*=40;
    r-=20;
    return r;
}

glm::vec3 Terrain::getNormal(int xpos, int ypos) {
    float heightL, heightR;
    float heightU, heightD;
    heightL = getHeight(xpos-1, ypos);
    heightR = getHeight(xpos+1, ypos);
    heightU = getHeight(xpos, ypos+1);
    heightD = getHeight(xpos, ypos-1);
    glm::vec3 normal(heightL-heightR,2.0f,heightD-heightU);
    return glm::normalize(normal);
}



