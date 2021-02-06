#pragma once

#include <SceneInterface.h>

class SceneCrazyDisco : public SceneInterface
{
public: 
	SceneCrazyDisco();
	void render() override;
	~SceneCrazyDisco() override;
};
