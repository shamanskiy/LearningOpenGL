// some funny preprocessor command to enable stb_image library
#define STB_IMAGE_IMPLEMENTATION

#include "Model.h"

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
		const char* what() const _NOEXCEPT
		{
			return "Texture file not found";
		}
	};
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

void Texture::activate() const
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
	m_name(modelName)
{
	resetBoundingBox();
	loadModel();
}

void Model::resetBoundingBox()
{
	m_boundingBox[0] = numeric_limits<GLfloat>::max();
	m_boundingBox[1] = numeric_limits<GLfloat>::min();
	m_boundingBox[2] = numeric_limits<GLfloat>::max();
	m_boundingBox[3] = numeric_limits<GLfloat>::min();
	m_boundingBox[4] = numeric_limits<GLfloat>::max();
	m_boundingBox[5] = numeric_limits<GLfloat>::min();
}

void Model::loadModel()
{
	Assimp::Importer importer;
	// flag to remove normals during import
	importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
	// edge detection for smooth normal generation
	importer.SetPropertyFloat("PP_GSN_MAX_SMOOTHING_ANGLE", 30);
	const aiScene* scene = importer.ReadFile(MODELS_DIR + m_name + "/" + m_name + ".obj",
		aiProcess_Triangulate | aiProcess_FlipUVs | 
		aiProcess_RemoveComponent |aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);
	if (!scene)
		throw ModelException("Failed to load a model: " + string(importer.GetErrorString()));

	loadNode(scene->mRootNode, scene);
	loadMaterials(scene);
}

void Model::loadNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (size_t i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i], scene);
}

void Model::updateBoundingBox(GLfloat x, int dim)
{
	if (x < m_boundingBox[2 * dim])
		m_boundingBox[2 * dim] = x;
	if (x > m_boundingBox[2 * dim + 1])
		m_boundingBox[2 * dim + 1] = x;
}

string Model::boundingBoxAsString() const
{
	string result = m_name + "/BoundingBox\n";
	result += "x: " + to_string(m_boundingBox[0]) + " " + to_string(m_boundingBox[1]) + "\n";
	result += "y: " + to_string(m_boundingBox[2]) + " " + to_string(m_boundingBox[3]) + "\n";
	result += "z: " + to_string(m_boundingBox[4]) + " " + to_string(m_boundingBox[5]);

	return result;
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
		updateBoundingBox(vertices[8 * i], 0);
		vertices[8 * i + 1] = mesh->mVertices[i].y;
		updateBoundingBox(vertices[8 * i + 1], 1);
		vertices[8 * i + 2] = mesh->mVertices[i].z;
		updateBoundingBox(vertices[8 * i + 2], 2);
		
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
	m_meshes.emplace_back(vertices, indices, 
		VertexData::POSITION | VertexData::UV | VertexData::NORMAL);
	// save a texture index that this Mesh uses
	m_meshToMaterial.push_back(mesh->mMaterialIndex);
}

Texture Model::loadTexture(aiMaterial* material) const
{
	if (material->GetTextureCount(aiTextureType_DIFFUSE))
	{
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		try
		{
			return Texture(MODELS_DIR + m_name + "/" + path.data);
		}
		catch (const TextureException& ex)
		{
			debugOutput(m_name + "/" + path.data + ": file not found.");
		}
	}
	
	debugOutput(m_name + "/" + string(material->GetName().data) +
			": using default texture.");
	return Texture(TEXTURES_DIR + "default.png");
}

void Model::loadMaterials(const aiScene* scene)
{
	m_materials.resize(scene->mNumMaterials);
	
	for (GLuint i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		Texture texture = loadTexture(material);

		aiColor3D color;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

		GLfloat shininess;
		material->Get(AI_MATKEY_SHININESS, shininess);

		m_materials[i] = Material{ 
			move(texture),
			glm::vec3{color.r,color.g,color.b},
			shininess };
	}
}

void Model::render(GLuint shader) const
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		m_materials[m_meshToMaterial[i]].activate(shader);
		//m_materials[m_meshToMaterial[i]]->activate();
		m_meshes[i].render();
	}
}

// ==============================================================================
// ==============          MATERIAL CLASS     ===================================
// ==============================================================================

void Material::activate(GLuint shader) const
{
	m_texture.activate();
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), m_shininess);
	glUniform3f(glGetUniformLocation(shader, "material.diffuseColor"),
		m_diffuseColor.x, m_diffuseColor.y, m_diffuseColor.z);
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

void ModelInstance::render(GLuint shader) const
{
	// send model matrix to the GPU
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
		glm::value_ptr(m_modelMatrix));
	// render the model
	m_model.render(shader);
}

