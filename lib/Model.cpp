#include "Model.h"

#include <iostream>

#include "Config.h"

using namespace std;

void Model::loadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);
	if (!scene)
	{
		std::cout << "Model " << fileName << "failed to load: "
			<< importer.GetErrorString() << std::endl;
		return;
	}

	loadNode(scene->mRootNode, scene);

	loadMaterials(scene);
}


void Model::loadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene);

	for (size_t i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i], scene);
}


void Model::loadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,
			mesh->mTextureCoords[0][i].y});
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z });

	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);


	}
	meshList.push_back(make_unique<Mesh>(vertices, indices));
	meshToTex.push_back(mesh->mMaterialIndex);
}


void Model::loadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				size_t idx = std::string(path.data).rfind("\\");
				std::string fileName = std::string(path.data).substr(idx + 1);

				std::string texPath = TEXTURES_DIR + fileName;

				textureList[i] = new Texture(texPath);
			}
		}

		if (textureList[i] == nullptr)
			textureList[i] = new Texture(TEXTURES_DIR + "grass.png");
	}
}

void Model::clearModel()
{

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

void Model::renderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->useTexture();
		}

		meshList[i]->renderMesh();
	}
}