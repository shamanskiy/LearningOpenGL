#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Config.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Outcome Model::loadModel(const string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);
	if (!scene)
		return Outcome(false, "Model " + fileName + " failed to load: "
			+ importer.GetErrorString());

	loadNode(scene->mRootNode, scene);
	loadMaterialsAndTextures(scene);

	return Outcome(true);
}

void Model::loadNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (size_t i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i], scene);
}


void Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<GLfloat> vertices;
	vector<GLuint> indices;

	// scan vertices (3 3D-coordinates, 2 uv-coordinates, 3 normals)
	vertices.resize(8 * mesh->mNumVertices, 0.);
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		vertices[8 * i] = mesh->mVertices[i].x;
		vertices[8 * i + 1] = mesh->mVertices[i].y;
		vertices[8 * i + 2] = mesh->mVertices[i].z;
		
		// get uv coordinates if provided
		if (mesh->mTextureCoords[0])
		{
			vertices[8 * i + 3] = mesh->mTextureCoords[0][i].x;
			vertices[8 * i + 4] = mesh->mTextureCoords[0][i].y;
		}
		
		// get vertex normals
		vertices[8 * i + 5] = mesh->mNormals[i].x;
		vertices[8 * i + 6] = mesh->mNormals[i].y;
		vertices[8 * i + 7] = mesh->mNormals[i].z;
	}

	// scan faces (triplets of vertex indices)
	indices.resize(3 * mesh->mNumFaces);
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
		for (GLuint j = 0; j < 3; j++)
			indices[3 * i + j] = mesh->mFaces[i].mIndices[j];

	// create a Mesh from vertices and indices
	m_meshes.push_back(make_unique<Mesh>(vertices, indices));
	// save a texture index that this Mesh uses
	m_meshToTexture.push_back(mesh->mMaterialIndex);
}


void Model::loadMaterialsAndTextures(const aiScene* scene)
{
	m_textures.resize(scene->mNumMaterials);

	for (GLuint i = 0; i < scene->mNumMaterials; i++)
		// Load texture file if provided
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			m_textures[i] = make_unique<Texture>(TEXTURES_DIR + path.data);
		}
		else // otherwise, use a default white texture
			m_textures[i] = make_unique<Texture>(TEXTURES_DIR + "default.png");
}

void Model::render() const
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_textures[m_meshToTexture[i]]->useTexture();
		//m_materials[m_meshToMaterial[i]]->activate();
		m_meshes[i]->renderMesh();
	}
}

ModelInstance::ModelInstance(const Model* const model,
	GLfloat posX, GLfloat posY, GLfloat posZ,
	GLfloat scale) :
	m_model(model),
	m_modelMatrix(glm::mat4(1.0f))
{
	m_modelMatrix = glm::translate(m_modelMatrix,
		glm::vec3(posX, posY, posZ));
	m_modelMatrix = glm::scale(m_modelMatrix,
		glm::vec3(scale, scale, scale));
}

void ModelInstance::render(const Shader& shader) const
{
	// send model matrix to the GPU
	glUniformMatrix4fv(shader.uniModelMatrix(), 1, GL_FALSE,
		glm::value_ptr(m_modelMatrix));
	// render the model
	m_model->render();
}
