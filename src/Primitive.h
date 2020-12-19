#pragma once

#include <memory>

class Mesh;

std::unique_ptr<Mesh> makePlane();
std::unique_ptr<Mesh> makeCube();
std::unique_ptr<Mesh> makePyramid();
