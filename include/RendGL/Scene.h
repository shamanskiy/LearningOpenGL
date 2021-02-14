#pragma once

#include <memory>

#include <Utils.h>

class Scene
{
public:
	virtual void render(const EventContainer & events) = 0;
	virtual ~Scene() {}

	static std::pair<std::unique_ptr<Scene>, Outcome> loadScene(const std::string& fileName);
};
