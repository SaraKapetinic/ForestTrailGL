//
// Created by predrag on 17.3.22..
//

#ifndef FORESTTRAILGL_SKYBOX_H
#define FORESTTRAILGL_SKYBOX_H

#include <vector>
#include <string>

class SkyBox {
public:
    SkyBox(std::vector<std::string> &faces, std::vector<std::string> &nightFaces);
    SkyBox();
    void Draw();

private:
    unsigned int VAO, VBO;
    unsigned dayTextureID;
    unsigned nightTextureID;
    void loadCubemapTextures(std::vector<std::string> &faces, unsigned &textureID);
};


#endif //FORESTTRAILGL_SKYBOX_H
