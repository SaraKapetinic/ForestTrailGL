//
// Created by predrag on 17.3.22..
//

#ifndef CGRAPHICS_SKYBOX_H
#define CGRAPHICS_SKYBOX_H

#include <vector>
#include <string>

class SkyBox {
public:
    SkyBox(std::vector<std::string> &faces);

    void Draw();

private:
    unsigned int VAO, VBO;
    unsigned textureID;
    void loadCubemapTextures(std::vector<std::string> &faces);
};


#endif //CGRAPHICS_SKYBOX_H
