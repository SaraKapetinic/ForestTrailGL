//
// Created by predrag on 18.2.22..
//

#ifndef CGRAPHICS_BRIDGEMODEL_H
#define CGRAPHICS_BRIDGEMODEL_H
#include "Shader.h"
#include <vector>
#include "bridge_mesh.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class BridgeModel {
public:
    BridgeModel(std::string path){
        loadModel(path);
    }
    void Draw(Shader &shader);

private:
    std::vector<bridge_mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene* scene);
    bridge_mesh processMesh(aiMesh *mesh, const aiScene* scene);
    unsigned TextureFromFile(const char *path, const std::string &directory);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //CGRAPHICS_BRIDGEMODEL_H
