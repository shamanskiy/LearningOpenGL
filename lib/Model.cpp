// some funny preprocessor command to enable stb_image library
#define STB_IMAGE_IMPLEMENTATION

#include "Model.h"

#include <stdexcept>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Config.h"

// ==============================================================================
// =======================          Helpers     =============================
// ==============================================================================

namespace
{
	struct TextureException : public std::exception
	{
		const char* what() const
		{
			return "Texture file not found";
		}
	};
}

// ==============================================================================
// =====================          MESH CLASS       ==============================
// ==============================================================================

Mesh::Mesh(const std::vector<GLfloat>& vertices,
	const std::vector<GLuint>& indices)
{
	createMesh(vertices, indices);
}

Mesh::~Mesh()
{
	deleteMesh();
}

Mesh::Mesh(Mesh&& other) noexcept :
	m_VAO(other.m_VAO),
	m_VBO(other.m_VBO),
	m_EBO(other.m_EBO),
	m_numIndices(other.m_numIndices)
{
	other.m_VAO = 0;
	other.m_VBO = 0;
	other.m_EBO = 0;
}

Mesh& Mesh::operator=(Mesh&& other) & noexcept
{
	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
	m_EBO = other.m_EBO;
	m_numIndices = other.m_numIndices;

	other.m_VAO = 0;
	other.m_VBO = 0;
	other.m_EBO = 0;

	return *this;
}

void Mesh::createMesh(const vector<GLfloat>& vertices,
	const vector<GLuint>& indices)
{
	GLuint numVertices = vertices.size();
	m_numIndices = indices.size();

	// create a Vertex Array Object on the GPU and store its number
	glGenVertexArrays(1, &m_VAO);
	// activate VAO to add further objects to it
	glBindVertexArray(m_VAO);

	// create a Vertex Buffer Object on the GPU and store its number
	glGenBuffers(1, &m_VBO);
	// activate VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// copy vertices to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, &vertices[0], GL_STATIC_DRAW);

	// we copy 5 values per vertex: the first 3 are the coordinates in 3D, the last 2 are the corresponding texture coordinates
	// here we tell OpenGl which values are coordinates
	// layout -> size of data (3 coordinates) -> type -> normalize -> stride -> offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	// enable (layout = 0), no need to search to the vertices when using the vertex shader (?)
	glEnableVertexAttribArray(0);

	// here we tell OpenGL which values are texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	// same as above
	glEnableVertexAttribArray(1);

	// here we tell OpenGL which values are normal components
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	// same as above
	glEnableVertexAttribArray(2);

	// create a Element Buffer Object on the GPU and store its number
	glGenBuffers(1, &m_EBO);
	// activate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// copy elements' indices to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * m_numIndices, &indices[0], GL_STATIC_DRAW);

	// deactivate VAO, no need to edit this object (should it be here or after VBO and EBO?)
	glBindVertexArray(0);
	// deactivate VBO, no need to edit the vertex data any more
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// deactivate EBO, no need to edit the index data any more
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::render() const
{
	// activate VAO, VAO = object (?)
	glBindVertexArray(m_VAO);
	// activate EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// 0 is a nullptr to smth
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
	// deactivate VAO and EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::deleteMesh()
{
	// free GPU memory
	if (m_EBO != 0)
		glDeleteBuffers(1, &m_EBO);
	if (m_VBO != 0)
		glDeleteBuffers(1, &m_VBO);
	if (m_VAO != 0)
		glDeleteVertexArrays(1, &m_VAO);
}

// ==============================================================================
// =====================       TEXTURE CLASS       ==============================
// ==============================================================================

Texture::Texture(const string& fileName) :
	m_textureID(0)
{
	loadTexture(fileName);
}

Texture::~Texture()
{
	deleteTexture();
}

Texture::Texture(Texture&& other) noexcept :
	m_textureID(other.m_textureID)
{
	other.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& other) & noexcept
{
	m_textureID = other.m_textureID;
	other.m_textureID = 0;
	return *this;
}

void Texture::loadTexture(const string& fileName)
{
	// image properties
	GLint width, height, bitDepth;

	// load image/texture data
	unsigned char* textureData = stbi_load(fileName.c_str(), &width, &height, &bitDepth, 0);
	if (!textureData)
		throw TextureException();

	// create texture object on the GPU
	glGenTextures(1, &m_textureID);
	// activate/bind texture object for future operations
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // x-wrap option (s is x for textures) GL_REPEAT GL_MIRRORED_REPEAT GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // y-wrap option (t is y for textures)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // texture scaling on zoom-out (related to mipmaps?) GL_LINEAR or GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // texture scaling on zoom-in (related to mipmaps?)

	glTexImage2D(GL_TEXTURE_2D, 0, // mipmap level
		GL_RGBA, width, height, // original image properties
		0, // this zero is a legacy parameter for handling borders
		GL_RGBA, // image format again (why?)
		GL_UNSIGNED_BYTE, textureData); // data type and data itself
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);
}

void Texture::activateTexture() const
{
	// texture unit - this guy will access texture data. 0 is default, so this is line is not necessary.
	// by using several different texture units we can bind several textures (?)
	glActiveTexture(GL_TEXTURE0);

	// bind this texture to the texture unit 0
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::deleteTexture()
{
	if (m_textureID != 0)
		glDeleteTextures(1, &m_textureID);
}


// ==============================================================================
// =======================          MODEL CLASS     =============================
// ==============================================================================

Model::Model(const string& modelName) :
	m_name(modelName),
	m_meshes(),
	m_textures(),
	m_meshToTexture()
{
	loadModel();
}

void Model::loadModel()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(MODELS_DIR + m_name + "/" + m_name + ".obj",
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);
	if (!scene)
		throw runtime_error("Failed to load a model: " + string(importer.GetErrorString()));

	loadNode(scene->mRootNode, scene);
	loadMaterialsAndTextures(scene);
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
	m_meshes.push_back(Mesh(vertices, indices));
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
			try
			{
				m_textures[i] = Texture(MODELS_DIR + m_name + "/" + path.data);
			}
			catch(const TextureException & ex)
			{
				debugOutput(m_name + "/" + path.data + ": file not found. Using a default texture.");
				m_textures[i] = Texture(TEXTURES_DIR + "default.png");
			}
		}
		else // otherwise, use a default white texture
		{
			debugOutput(m_name + "/" + string(scene->mMaterials[i]->GetName().data) + 
				": no texture found. Using a default texture.");
			m_textures[i] = Texture(TEXTURES_DIR + "default.png");
		}
}

void Model::render() const
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_textures[m_meshToTexture[i]].activateTexture();
		//m_materials[m_meshToMaterial[i]]->activate();
		m_meshes[i].render();
	}
}

// ==============================================================================
// ==============          MODEL INSTANCE CLASS     =============================
// ==============================================================================

ModelInstance::ModelInstance(const Model& model,
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
	m_model.render();
}

