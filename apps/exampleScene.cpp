#include <iostream>

#include "Window.h"

int main() {

    // create & initialize main window with dimensions
    Window window(800,600);
    auto out = window.initialize();
    if (!out.ok())
    {
        std::cout << out.message() << std::endl;
        return 1;
    }

    // Loop until window closed
    while (!window.shouldClose())
    {
        // Get + Handle user input events
        window.pollEvents();

        window.swapBuffers();
    }

    return 0;
}
