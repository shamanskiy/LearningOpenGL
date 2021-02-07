#pragma once

#include <Scene.h>

class SceneCrazyDisco : public Scene
{
public: 
	SceneCrazyDisco();
	void render(const InputContainer& input) override;
	~SceneCrazyDisco() override;
};
