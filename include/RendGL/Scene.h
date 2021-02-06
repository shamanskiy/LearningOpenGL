#pragma once

#include <memory>
#include <string>
class Scene
{
public:
	virtual void render() = 0;
	virtual ~Scene() {}

	static std::unique_ptr<Scene> loadScene(const std::string& fileName);
};
