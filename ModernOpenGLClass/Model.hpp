#ifndef Model_hpp
#define Model_hpp

#include <vector>
#include <string>
#include <stdio.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Texture.hpp"

class Model
{
public:
    Model();
    
    void loadModel(const std::string& fileName);
    void renderModel();
    void clearModel();

    ~Model();
    
private:
    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<unsigned int> meshToTex;
    
    void loadNode(aiNode *node, const aiScene *scene);
    void loadMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterials(const aiScene *scene);
};

#endif /* Model_hpp */
