#pragma once

#include <Scene.h>

class SceneCrazyDisco : public Scene
{
public: 
	SceneCrazyDisco();
	void render(const EventContainer& input) override;
	~SceneCrazyDisco() override;
};
