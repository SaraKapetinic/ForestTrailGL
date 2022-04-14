//
// Created by predrag on 1.3.22..
//

#include "TerrainModel.h"
#include "../../../external/stb_image.h"

void TerrainModel::Draw(Shader& shader) {

    shader.setInt("grassText",0);
    shader.setInt("dirtText",1);
    shader.setInt("blendMap",2);
    shader.setInt("stoneText", 3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, terrainTex[0].id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, terrainTex[1].id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, terrainTex[2].id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, terrainTex[3].id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
}

void TerrainModel::loadTerrainTextures(std::vector<const char *> &paths) {
    for(int i=0; i < paths.size();i++){
        std::string filename = std::string(paths[i]);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << paths[i] << std::endl;
            stbi_image_free(data);
        }

        terrainTex[i].id = textureID;
        terrainTex[i].path = paths[i];
    }
}
