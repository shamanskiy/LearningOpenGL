#pragma once

#include <vector>

#include "Scene.h"
#include "Light.h"
#include "Camera.h"

class Model;
class ModelInstance;
class Shader;

class Scene3D : public Scene
{
public: 
	Scene3D();
	void render(const EventContainer& events) override;
	~Scene3D() override;

private:

	Light m_light;
	Camera m_camera;

	std::vector<std::unique_ptr<Model> > m_models;
	std::vector<std::unique_ptr<ModelInstance> > m_instances;
	std::vector<std::unique_ptr<Shader> > m_shaders;
};
