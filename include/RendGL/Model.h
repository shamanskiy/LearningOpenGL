#pragma once

#include <vector>
#include <array>
#include <string>
#include <memory>

// must be here for some reason; can't move to Texture.cpp
#include <stb_image.h>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "Utils.h"

using namespace std;

class Shader;

// Mesh represents a single 3D object. It gets an array of vertices
// and an array of indices, copies them to the GPU and then holds
// the pointers to the arrays of the GPU.
class Mesh
{
public:
	Mesh(const vector<GLfloat>& vertices,
		const vector<GLuint>& indices);
	~Mesh();

	// render object
	void render() const;

private:
	// create arrays on the GPU
	void createMesh(const vector<GLfloat>& vertices,
		const vector<GLuint>& indices);

	// delete object from GPU memory
	void deleteMesh();

private:
	// Vertex Array, Vertex Buffer and Element Buffer Objects
	// hold reference numbers to data objects on GPU
	GLuint m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 };
	// total number of indices to draw = num of elements * 3
	GLuint m_numIndices{ 0 };
};

// Texture is a class for loading and managing textures
class Texture
{
private:
	// ID of the texture object on the GPU
	GLuint m_textureID;

public:
	// ctor: create texture on the GPU
	Texture(const string& fileName);
	// dtor: free memory on GPU
	~Texture();
	// activate/bind texture on the GPU
	void useTexture() const;

private:
	// load texture from file
	void loadTexture(const string& fileName);
	// free memory on the GPU
	void clearTexture();
};



// Model represent a 3D model stored in a file.
// It can contain several Meshes, one for each part of the Model.
// For each Mesh, there is a Texture and a Material.
class Model
{
public:
	// Constructor doesn't do anything. I use a separate function
	// to actually load the model to be able to return error objects
	// without using exceptions.
	Model(const string& modelName);

	// Render all meshes stored in the model using the corresponding 
	// textures and materials.
	void render() const;

private:
	// open model file and start loading nodes
	void loadModel();
	// Recursive function that parses the model tree and loads meshes from each node.
	void loadNode(aiNode* node, const aiScene* scene);
	// Load a mesh
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	// Load all materials and textures stored in the model.
	void loadMaterialsAndTextures(const aiScene* scene);

private:
	// name of the folder where the model files are stored
	string m_name;
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



// ModelInstance represents an instance of a 3D model
// that is scaled and located at a given position.
class ModelInstance
{
public:
	ModelInstance(const Model* const model,
		GLfloat posX = 0.0f, GLfloat posY = 0.0f, GLfloat posZ = 0.0f,
		GLfloat scale = 1.0f);

	void render(const Shader& shader) const;

private:
	// a non-owning pointer to the Model
	const Model* const m_model;
	// model matrix (translation + scale)
	glm::mat4 m_modelMatrix;
};