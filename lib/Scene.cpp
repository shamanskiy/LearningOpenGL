#include "Scene.h"
#include "SceneCrazyDisco.h"

std::unique_ptr<Scene> Scene::loadScene(const std::string& fileName)
{
	return std::make_unique<SceneCrazyDisco>();
}
