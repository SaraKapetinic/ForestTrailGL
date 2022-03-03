//
// Created by predrag on 27.2.22..
//

#include "Terrain.h"
#include <iostream>

TerrainModel Terrain::generateTerrain() {
    indices.resize(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));


    for(int i=0;i<VERTEX_COUNT;i++){
        for(int j=0;j<VERTEX_COUNT;j++){
            glm::vec3 Position((float)j/((float)VERTEX_COUNT - 1) * SIZE, 0, (float)i/((float)VERTEX_COUNT - 1) * SIZE);
            glm::vec3 Normal(0.0f,1.0f,0.0f);
            glm::vec2 TextureCords((float)j/((float)VERTEX_COUNT - 1), (float)i/((float)VERTEX_COUNT - 1));
            Vertex v;
            v.Position = Position;
            v.Normal = Normal;
            v.TexCoords = TextureCords;
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

    return TerrainModel(vertices, indices);

}

