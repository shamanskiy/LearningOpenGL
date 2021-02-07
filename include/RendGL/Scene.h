#pragma once

#include <memory>
#include <string>

#include <Utils.h>

class Scene
{
public:
	virtual void render(const EventContainer & input) = 0;
	virtual ~Scene() {}

	static std::pair<std::unique_ptr<Scene>, Outcome> loadScene(const std::string& fileName);
};