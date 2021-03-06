#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>

#include <assimp/scene.h>

#include "Mesh.h"
#include "Texture.h"
#include "Utils.h"

using namespace std;

// Model represent a 3D model stored in a file.
// It can contain several Meshes, one for each part of the Model.
// For each Mesh, there is a Texture and a Material.
class Model
{
public:
	// Constructor doesn't do anything. I use a separate function
	// to actually load the model to be able to return error objects
	// without using exceptions.
	Model() = default;
	Outcome loadModel(const string& fileName);

	// Render all meshes stored in the model using the corresponding 
	// textures and materials.
	void renderModel();

private:
	// Recursive function that parses the model tree and loads meshes from each node.
	void loadNode(aiNode* node, const aiScene* scene);
	// Load a mesh
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	// Load all materials and textures stored in the model.
	void loadMaterialsAndTextures(const aiScene* scene);

private:
	// List of Meshes (parts) that form the Model
	vector<unique_ptr<Mesh> > m_meshes;
	// List of Textures to use for different Meshes
	vector<unique_ptr<Texture> > m_textures;
	// Mapping between Mesh and Texture indices
	vector<GLuint> m_meshToTexture;
	// List of Materials to use for different Meshes
	//vector<unique_ptr<Material> > m_materials;
	// Mapping between Mesh and Material indices
	//vector<GLuint> m_meshToMaterial;

	//array<GLfloat, 6> m_boundingBox;
};