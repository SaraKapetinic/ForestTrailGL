//
// Created by predrag on 27.2.22..
//

#include "Terrain.h"

Mesh Terrain::generateTerrain() {
    int count = VERTEX_COUNT * VERTEX_COUNT;
    vertices.resize(count*3);
    normals.resize(count * 3);
    textureCords.resize(count*2);

    indices.resize(6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1));

    for(int i=0;i<VERTEX_COUNT;i++){
        for(int j=0;j<VERTEX_COUNT;j++){
            glm::vec3 Position((float)j/((float)VERTEX_COUNT - 1) * SIZE, 0, (float)i/((float)VERTEX_COUNT - 1) * SIZE);
            glm::vec3 Normal(0.0f,1.0f,0.0f);
            glm::vec2 TextureCords((float)j/((float)VERTEX_COUNT - 1), (float)i/((float)VERTEX_COUNT - 1));
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
    std::vector<Texture> texture;

    texture.push_back(loadTerrainTexture("../resources/TERRAINTEX.JPG"));
    return Mesh(vertices,indices,texture);
}

Texture Terrain::loadTerrainTexture(const char *path){
    std::string filename = std::string(path);

    unsigned textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    Texture tex;
    tex.id = textureID;
    tex.type = "texture_diffuse";
    tex.path = path;
    return tex;
}

const Mesh &Terrain::getTerrainMesh() const {
    return terrainMesh;
}
