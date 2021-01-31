#pragma once

#include <memory>

class Mesh;

// create a horizonal 1x1 square centered at the origin
std::unique_ptr<Mesh> makePlane();
// create a 1x1x1 cube centered at the origin
std::unique_ptr<Mesh> makeCube();
// create a square pyramid with 1x1 base centered at the origin
// and a height of 0.5
std::unique_ptr<Mesh> makePyramid();
