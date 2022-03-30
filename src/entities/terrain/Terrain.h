//
// Created by predrag on 27.2.22..
//

#ifndef FORESTTRAILGL_TERRAIN_H
#define FORESTTRAILGL_TERRAIN_H
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "TerrainModel.h"

class Terrain {
public:
    Terrain(int gridX, int gridZ, float terrainScale) {
        x = gridX * SIZE;
        z = gridZ * SIZE;
        scale = terrainScale;
    }

    TerrainModel generateTerrain();
private:
    float SIZE = 1024;
public:
    float getSize() const;

private:
    int VERTEX_COUNT;

    float x;
    float z;
    float scale;
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    int heightMapHeight;
    int heightMapWidth;

    unsigned char* heightMap;

    void loadHeightMap(std::string path);
    float getHeight(int x, int y);
    glm::vec3 getNormal(int x, int y);
};


#endif //FORESTTRAILGL_TERRAIN_H
