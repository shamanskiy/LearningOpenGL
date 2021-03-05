#pragma once

#include <vector>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model() {};

	void loadModel(const std::string& fileName);
	void renderModel();
	void clearModel();


private:

	void loadNode(aiNode* node, const aiScene* scene);
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterials(const aiScene* scene);

	std::vector<std::unique_ptr<Mesh> > meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;


};