//
// Created by predrag on 18.2.22..
//

#ifndef CGRAPHICS_MODEL_H
#define CGRAPHICS_MODEL_H
#include "../../Shader.h"
#include <vector>
#include "Mesh.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../../external/stb_image.h"

class Model {
public:
    Model(std::string path){

        loadModel(path);

    }
    void Draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene* scene);
    Mesh processMesh(aiMesh *mesh, const aiScene* scene);
    unsigned TextureFromFile(const char *path, const std::string &directory);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif //CGRAPHICS_MODEL_H