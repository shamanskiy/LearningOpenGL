#include <iostream>

#include "Window.h"
#include "SceneCrazyDisco.h"

std::unique_ptr<SceneInterface> createScene()
{
    return std::make_unique<SceneCrazyDisco>();
}


int main() {

    // create & initialize main window with dimensions
    Window window(800,600);
    auto out = window.initialize();
    if (!out.ok())
    {
        std::cout << out.message() << std::endl;
        return 1;
    }

    // Scene holds all information about models, textures and lights to render
    // as well as the camera
    auto scene = createScene();

    // Loop until window closed
    while (!window.shouldClose())
    {
        // Get + Handle user input events
        window.pollEvents();

        scene->render();

        window.swapBuffers();
    }

    return 0;
}
