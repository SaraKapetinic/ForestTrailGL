//
// Created by predrag on 28.3.22..
//

#include "Water.h"

WaterModel Water::generateWater() {


    VERTEX_COUNT = 128;
    indices.resize(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));


    for(int i=0;i<VERTEX_COUNT;i++){
        for(int j=0;j<VERTEX_COUNT;j++){
            float x, z;

            x = (float)j/((float)VERTEX_COUNT - 1);
            z = (float)i/((float)VERTEX_COUNT - 1);
            //std::cout << getHeight(j,i) << std::endl;
            glm::vec3 Position( x * SIZE, 0,   z * SIZE);
            glm::vec3 Normal(0.0f, 1.0f, 0.0f);
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


    return WaterModel(vertices, indices);

}

float Water::getSize() const {
    return SIZE;
}
