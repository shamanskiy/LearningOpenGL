#include "Scene3D.h"

#include <iostream>
#include <GL/glew.h>

Scene3D::Scene3D() {}

void Scene3D::render(const EventContainer& input) {
    glClearColor(1.0, 1.0, 0.0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

Scene3D::~Scene3D() {}