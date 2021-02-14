#pragma once

#include <vector>

#include "Scene.h"
#include "Light.h"
#include "Camera.h"

class Mesh;
class Texture;
class StaticModel;
class Shader;

class Scene3D : public Scene
{
public: 
	Scene3D();
	void render(const EventContainer& events) override;
	~Scene3D() override;

private:

	Light light;
	Camera camera;

	std::vector<std::unique_ptr<Mesh> > meshes;
	std::vector<std::unique_ptr<Texture> > textures;
	std::vector<std::unique_ptr<StaticModel> > models;
	std::vector<std::unique_ptr<Shader> > shaders;

	bool shaderSwitcher;
};
