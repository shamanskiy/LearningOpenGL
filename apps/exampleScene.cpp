#include <iostream>

#include "Window.h"

int main() {

    // create & initialize main window with dimensions
    Window mainWindow(800,600);
    auto out = mainWindow.initialize();
    if (!out.ok())
    {
        std::cout << out.message() << std::endl;
        return 1;
    }

    // Loop until window closed
    while (!mainWindow.shouldClose())
    {
        // Get + Handle user input events
        glfwPollEvents();
    }

    return 0;
}
