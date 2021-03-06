#pragma once

#include <vector>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

// Model represent a 3D model stored in a file.
// It can contain several Meshes, one for each part of the Model.
// For each Mesh, there is a Texture and a Material.
class Model
{
public:
	Model() {};

	void loadModel(const std::string& fileName);
	void renderModel();

private:

	void loadNode(aiNode* node, const aiScene* scene);
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	void loadMaterials(const aiScene* scene);

private:
	// List of Meshes (parts) that form the Model
	std::vector<std::unique_ptr<Mesh> > m_meshes;
	// List of Textures to use for different Meshes
	std::vector<std::unique_ptr<Texture> > m_textures;
	// Mapping between Mesh and Texture indices
	std::vector<GLuint> m_meshToTexture;
};