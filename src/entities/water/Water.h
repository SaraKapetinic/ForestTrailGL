//
// Created by predrag on 28.3.22..
//

#ifndef FORESTTRAILGL_WATER_H
#define FORESTTRAILGL_WATER_H
#include "DataStructs.h"
#include "WaterModel.h"

class Water {
public:
    Water(int gridX, int gridZ, float waterScale){
        x = gridX * SIZE;
        z = gridZ * SIZE;
        scale = waterScale;
    }
    WaterModel generateWater();

private:
    float x, z ,scale;
    float SIZE = 1024;
public:
    float getSize() const;

private:
    int VERTEX_COUNT;
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
};


#endif //FORESTTRAILGL_WATER_H
