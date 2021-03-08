#pragma once

#include <memory>
#include <vector>

#include "Light.h"
#include "Camera.h"
#include <Utils.h>

class Scene
{
public:
	// Get fresh events and render the scene
	virtual void render(const EventContainer & events) = 0;
	virtual ~Scene() {}

	// Factory that loads a scene from a file
	static std::pair<std::unique_ptr<Scene>, Outcome> loadScene(const std::string& fileName);
};

class Model;
class ModelInstance;
class Shader;

class Scene3D : public Scene
{
public:
	Scene3D();
	~Scene3D() override;

	// Get fresh events and render the scene
	void render(const EventContainer& events) override;

private:
	// Interactive camera
	Camera m_camera;
	// List of shaders that can be used to render the scene.
	// Can be cycled through.
	std::vector<std::unique_ptr<Shader> > m_shaders;

	// List of Models used in the Scene
	std::vector<std::unique_ptr<Model> > m_models;
	// Instances of Models that are scaled and translated to positions
	std::vector<std::unique_ptr<ModelInstance> > m_instances;

	// Ambient + directional light
	Light m_light;
};
