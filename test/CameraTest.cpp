#include "gtest/gtest.h"
#include "Camera.h"

#include <glm/glm.hpp>




TEST(Camera, createCamera)
{
    Camera camera(glm::vec3(1.5f,2.5f,3.5f), // initial location
                  glm::vec3(0.0f,1.0f,0.0f), // up direction
                  -110.0f, // pitch: rotation in XY plane from the X axis
                  -20.0f, // yaw: rotation up-down from the horizontal
                  10.0f,  // linear move speed pixel/sec
                  0.05f); // rotational move speed a.k.a. mouse sensitivity
    ASSERT_TRUE(true);
}

TEST(Camera, createCamera2)
{
    Camera camera(glm::vec3(1.5f,2.5f,3.5f), // initial location
                  glm::vec3(0.0f,1.0f,0.0f), // up direction
                  -110.0f, // pitch: rotation in XY plane from the X axis
                  -20.0f, // yaw: rotation up-down from the horizontal
                  10.0f,  // linear move speed pixel/sec
                  0.05f); // rotational move speed a.k.a. mouse sensitivity
    ASSERT_TRUE(true);
}


