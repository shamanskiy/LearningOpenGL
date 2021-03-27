#pragma once

#include <memory>
#include <string>

class EventContainer;

using namespace std;

class Scene
{
public:
	// Get fresh events and render the scene
	virtual void render(const EventContainer & events) = 0;
	virtual ~Scene() = 0;

	// Factory that loads a scene from a file
	static std::unique_ptr<Scene> loadScene(const std::string& fileName);
};
