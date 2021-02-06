#include <iostream>

#include "Window.h"
#include "Scene.h"

int main() {

    // Create and initialize an application window with given dimensions.
    Window window(800,600);
    auto outcomeWindow = window.initialize();
    if (!outcomeWindow.ok())
    {
        std::cout << outcomeWindow.message() << std::endl;
        return 1;
    }

    // Scene holds all information about models, textures and lights to render
    // as well as the camera.
    auto [scene, outcomeLoad] = Scene::loadScene("fileNamePlaceHolder");
    if (!outcomeLoad.ok())
    {
        std::cout << outcomeLoad.message() << std::endl;
        return 1;
    }

    // Loop until the window is closed.
    while (!window.shouldClose())
    {
        // Process and accumulate mouse and keyboard events during this frame.
        window.pollEvents();

        // Scene makes all the calls to OpenGL that it need to do to render.
        // Receives input events from the window and extracts whatever is important.
        // Also gets a time step (dt, delta t) to update camera and animations.
        scene->render();

        // Window has been displaying a buffer from the previous frame.
        // OpenGL has been drawing to a reserve buffer.
        // Now swap the buffers to update the image.
        window.swapBuffers();
    }

    return 0;
}
