#include "Scene.h"
#include "SceneCrazyDisco.h"

std::pair<std::unique_ptr<Scene>, Outcome> Scene::loadScene(const std::string& fileName)
{
	return std::make_pair(std::make_unique<SceneCrazyDisco>(), Outcome(true));
}
