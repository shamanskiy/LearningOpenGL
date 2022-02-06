#pragma once

#include <vector>
#include <string>
#include <array>

// must be here for some reason; can't move to Texture.cpp
#include <stb_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"

using namespace std;

class aiNode;
class aiMesh;
class aiScene;
class aiMaterial;

// Texture loads an image from file to the GPU
// and holds a pointer to the data on the GPU
class Texture
{
public:
	// move-only
	Texture() = default;
	Texture(const string& fileName);
	~Texture();
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) & noexcept;

	// Activates texture. Any object rendered by the GPU will use this texture
	void activate() const;

private:
	// load from file to GPU
	void loadTexture(const string& fileName);
	// delete from GPU
	void deleteTexture();

private:
	// ID of the texture object on the GPU
	GLuint m_textureID;
};

struct Material
{
	Texture m_texture;
	glm::vec3 m_diffuseColor;
	GLfloat m_shininess;

	void activate(GLuint shader) const;
};

// Model represent a 3D model stored in a file.
// It can contain several Meshes, one for each part of the Model.
// For each Mesh, there is a Texture and a Material.
class Model
{
public:
	Model() = default;
	Model(const string& modelName);

	void render(GLuint shader) const;

	const array<GLfloat, 6>& boundingBox() const { return m_boundingBox; }
	string boundingBoxAsString() const;

private:
	// open model file and start loading nodes
	void loadModel();
	// Recursive function that parses the model tree and loads meshes from each node.
	void loadNode(aiNode* node, const aiScene* scene);
	// Load a mesh
	void loadMesh(aiMesh* mesh, const aiScene* scene);
	// Load all materials and textures stored in the model.
	void loadMaterials(const aiScene* scene);
	Texture loadTexture(aiMaterial* material) const;

	void resetBoundingBox();
	void updateBoundingBox(GLfloat x, int dim);

private:
	// name of the folder where the model files are stored
	string m_name;
	// List of Meshes (parts) that form the Model
	vector<Mesh> m_meshes;
	// List of Materials for different meshes
	vector<Material> m_materials;
	// Mapping between Mesh and Material indices
	vector<GLuint> m_meshToMaterial;
	
	array<GLfloat, 6> m_boundingBox;
};



// ModelInstance represents an instance of a 3D model
// that is scaled and located at a given position.
class ModelInstance
{
public:
	ModelInstance(const Model& model,
		GLfloat posX = 0.0f, GLfloat posY = 0.0f, GLfloat posZ = 0.0f,
		GLfloat scale = 1.0f);

	void render(GLuint shader) const;

private:
	// a non-owning pointer to the Model
	const Model& m_model;
	// model matrix (translation + scale)
	glm::mat4 m_modelMatrix;
};


