#pragma once

#include <Scene.h>

class Scene3D : public Scene
{
public: 
	Scene3D();
	void render(const EventContainer& input) override;
	~Scene3D() override;
};
