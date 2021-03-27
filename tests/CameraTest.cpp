#include "gtest/gtest.h"
#include "Camera.h"
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace {
    std::pair<Camera, EventContainer> setup()
    {
        auto result = std::make_pair(Camera(), EventContainer());
        result.second.setTime(0.0f);
        result.second.setTime(1.0f);
        return result;
    }
}

TEST(CameraTest, moveForwardUpRight)
{
    auto [camera, events] = setup();

    events.setKeyState(GLFW_KEY_W, true);
    events.setKeyState(GLFW_KEY_D, true);
    events.setKeyState(GLFW_KEY_UP, true);

    camera.processEvents(events);

    ASSERT_FLOAT_EQ(camera.position().x, 1.0f);
    ASSERT_FLOAT_EQ(camera.position().y, 1.0f);
    ASSERT_FLOAT_EQ(camera.position().z, 1.0f);
}

TEST(CameraTest, moveBackLeftDown)
{
    auto [camera, events] = setup();

    events.setKeyState(GLFW_KEY_S, true);
    events.setKeyState(GLFW_KEY_A, true);
    events.setKeyState(GLFW_KEY_DOWN, true);

    camera.processEvents(events);

    ASSERT_FLOAT_EQ(camera.position().x, -1.0f);
    ASSERT_FLOAT_EQ(camera.position().y, -1.0f);
    ASSERT_FLOAT_EQ(camera.position().z, -1.0f);
}

TEST(CameraTest, rotate45DegRight)
{
    auto [camera, events] = setup();

    events.setCursorPosition(0.0f, 0.0f);
    events.setCursorPosition(45.0f, 0.0f);

    camera.processEvents(events);

    auto frontDir = camera.front();
    ASSERT_FLOAT_EQ(frontDir.x, 1.0f/sqrt(2.0f));
    ASSERT_FLOAT_EQ(frontDir.y, 0.0f);
    ASSERT_FLOAT_EQ(frontDir.z, 1.0f/ sqrt(2.0f));
}

TEST(CameraTest, rotate45DegUp)
{
    auto [camera, events] = setup();

    events.setCursorPosition(0.0, 0.0);
    events.setCursorPosition(0.0, -45.0); // vertical inversion

    camera.processEvents(events);

    auto frontDir = camera.front();
    ASSERT_FLOAT_EQ(frontDir.x, 1.0f / sqrt(2.0f));
    ASSERT_FLOAT_EQ(frontDir.y, 1.0f / sqrt(2.0f));
    ASSERT_FLOAT_EQ(frontDir.z, 0.0f);
}

TEST(CameraTest, verticalRotationIsLimited)
{
    auto [camera, events] = setup();

    events.setCursorPosition(0.0, 0.0);
    events.setCursorPosition(0.0, -90.0); // vertical inversion

    camera.processEvents(events);

    auto frontDir = camera.front();
    ASSERT_TRUE(frontDir.x > 0.0f);
    ASSERT_TRUE(frontDir.y < 1.0f);
}


