#include "SceneCrazyDisco.h"

#include <iostream>
#include <GL/glew.h>

SceneCrazyDisco::SceneCrazyDisco() {}

void SceneCrazyDisco::render(const InputContainer& input) {
    glClearColor(1.0f / 255 * (rand() % 256), 1.0f / 255 * ((rand() + 100) % 256), 1.0f / 255 * ((rand() - 100) % 256), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

SceneCrazyDisco::~SceneCrazyDisco() {}