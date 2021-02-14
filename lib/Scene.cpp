#include "Scene.h"
#include "Scene3D.h"

std::pair<std::unique_ptr<Scene>, Outcome> Scene::loadScene(const std::string& fileName)
{
	return std::make_pair(std::make_unique<Scene3D>(), Outcome(true));
}
