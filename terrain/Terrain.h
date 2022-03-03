//
// Created by predrag on 27.2.22..
//

#ifndef CGRAPHICS_TERRAIN_H
#define CGRAPHICS_TERRAIN_H
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "TerrainModel.h"

class Terrain {
public:
    Terrain(int gridX, int gridZ){
        x = gridX * SIZE;
        z = gridZ * SIZE;
    }

    TerrainModel generateTerrain();
private:
    float SIZE = 800;
    int VERTEX_COUNT = 128;

    float x;
    float z;

    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;

};


#endif //CGRAPHICS_TERRAIN_H
