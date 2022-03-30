//
// Created by predrag on 30.3.22..
//

#ifndef FORESTTRAILGL_INSTANCEDMODEL_H
#define FORESTTRAILGL_INSTANCEDMODEL_H
#include "Shader.h"
#include <vector>
#include "InstancedMesh.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

class InstancedModel {
public:
    InstancedModel(std::string path){
        loadInstancedModel(path);
    }
    void Draw(Shader &shader, int n);

private:
    std::vector<InstancedMesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    void loadInstancedModel(std::string path);
    void processInstancedNode(aiNode *node, const aiScene* scene);
    InstancedMesh processInstancedMesh(aiMesh *mesh, const aiScene *scene);
    unsigned textureFromFile(const char *path);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};


#endif //FORESTTRAILGL_INSTANCEDMODEL_H
