#pragma once

#include <string>
#include <array>

#include <GL/glew.h>
#include <glfw/glfw3.h>

// Some operations return an Outcome.
// An outcome contains the operation status (true/false = success/fail)
// and a message if the operation has failed. 
class Outcome
{

public:
    // Constructor to use with no message (usually if ok)
    explicit Outcome(bool ok) :
        m_ok(ok), 
        m_message("") {}

    Outcome(bool ok, const std::string& message) :
        m_ok(ok),
        m_message(message) {}

    Outcome(bool ok, std::string&& message) :
        m_ok(ok),
        m_message(std::move(message)) {}

    // Operation status (true/false = success/fail)
    bool ok() const { return m_ok; }
    // Operation message if failed (empty if success)
    const std::string& message() const { return m_message; }

private:
    // True/false = success/fail
    bool m_ok;
    // Message if fail
    std::string m_message;
};

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

private:
    // Tracks time elapsed between frames
    TimeTracker m_timeTracker;

    // States of all keys this frame
    std::array<bool, 1024> m_keys;

    // Tracks relative movement of the cursor between frames
    CursorTracker m_cursorTracker;
};


