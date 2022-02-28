//
// Created by predrag on 27.2.22..
//

#ifndef CGRAPHICS_TERRAIN_H
#define CGRAPHICS_TERRAIN_H
#include <vector>
#include "../model/Model.h"
#include "../model/Mesh.h"

class Terrain {
public:
    Terrain(int gridX, int gridZ){
        x = gridX * SIZE;
        z = gridZ * SIZE;
    }

    Mesh generateTerrain();
private:
    float SIZE = 800;
    int VERTEX_COUNT = 128;

    float x;
    float z;

    std::vector<Vertex> vertices;
    std::vector<Vertex> normals;
    std::vector<Vertex> textureCords;
    std::vector<unsigned> indices;

    Texture loadTerrainTexture(const char* path);

    Mesh terrainMesh = generateTerrain();
public:
    const Mesh &getTerrainMesh() const;
};


#endif //CGRAPHICS_TERRAIN_H
