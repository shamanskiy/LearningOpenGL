#pragma once

#include <string>
#include <array>

#include <GL/glew.h>
#include <glfw/glfw3.h>


// TimeKeeper records current time and returns how much time has passed
// since the last time record. Returns zero time steps until the second time record.
class TimeTracker
{
public:
    TimeTracker();

    // Get time step between the last two time records
    GLfloat timeStep() const { return m_dt; }
    // Set current time and compute time step
    void setTime(GLfloat time);

private:
    // Current time
    GLfloat m_t;
    // Time passed since the last time record
    GLfloat m_dt;

    // Initialized with false; set true once setTime is called
    bool m_canComputeTimeStep;
};

// CursorTracker gets the current cursor position and tells how much
// the cursor has moved from the last known position. 
class CursorTracker
{
public:
    CursorTracker();

    // Get position change between the last two reported positions
    GLfloat positionChangeX() const { return m_dx; }
    GLfloat positionChangeY() const { return m_dy; }
    // Set current cursor position and compute position change
    void setPosition(GLfloat x, GLfloat y);

    // Set position change to zero to prevent cursor drifting when mouse is inactive
    void clearPositionChange();

private:
    // Current cursor position
    GLfloat m_x, m_y;
    // Cursor position change
    GLfloat m_dx, m_dy;

    // Initialized with false; set true once setTime is called
    bool m_canComputePositionChange;
}; 

class EventContainer
{
public:
    EventContainer();

    // Reset events before polling new events.
    // Currently only resets cursor position change.
    void reset();

    // Record current time to compute time step
    void setTime(GLfloat time) { m_timeTracker.setTime(time); }
    // Time passed since the last time record
    GLfloat timeStep() const { return m_timeTracker.timeStep(); }

    // Set key state. Uses GLFW key codes. True/false = down/up
    void setKeyState(int key, bool state) { m_keys[key] = state; }
    // Get key state. Uses GLFW key codes. True/false = down/up
    bool keyState(int key) const { return m_keys[key]; }

    // Set cursor position and compute position change
    void setCursorPosition(GLfloat x, GLfloat y) { m_cursorTracker.setPosition(x, y); }
    GLfloat cursorPositionChangeX() const  {return m_cursorTracker.positionChangeX(); }
    GLfloat cursorPositionChangeY() const { return m_cursorTracker.positionChangeY(); }

    GLfloat aspectRatio() const { return m_aspectRatio; }
    void setAspectRatio(GLfloat aRatio) { m_aspectRatio = aRatio; }

private:
    // Tracks time elapsed between frames
    TimeTracker m_timeTracker;

    // States of all keys this frame
    std::array<bool, 1024> m_keys;

    // Tracks relative movement of the cursor between frames
    CursorTracker m_cursorTracker;

    // Current image aspect ratio
    GLfloat m_aspectRatio;
};

// TODO: prints a message to the console in the debug build only
void debugOutput(const std::string& message);
