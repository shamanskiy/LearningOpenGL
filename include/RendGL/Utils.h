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

    // operation status (true/false = success/fail)
    bool ok() const { return m_ok; }
    // operation message if failed (empty if success)
    const std::string& message() const { return m_message; }

private:
    // true/false = success/fail
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

    // get time step between the last two time records
    GLfloat timeStep() const { return m_timeStep; }
    // set current time and compute time step
    void setTime(GLfloat time);

private:
    // current time record
    GLfloat m_time;
    // time passed since the last time record
    GLfloat m_timeStep;

    // initialized with false; set true once setTime is called
    bool m_canComputeTimeStep;
};


class EventContainer
{
public:
    EventContainer();

    // current time record
    void setTime(GLfloat time) { m_timeTracker.setTime(time); }
    // time passed since the last time record
    GLfloat timeStep() const { return m_timeTracker.timeStep(); }

    // set key state. Uses GLFW key codes. True/false = down/up
    void setKeyState(int key, bool state) { m_keys[key] = state; }
    // get key state. Uses GLFW key codes. True/false = down/up
    bool keyState(int key) const { return m_keys[key]; }

private:
    // time tracker computes time steps for other systems
    TimeTracker m_timeTracker;

    // states of all keys on the keyboard in ASCII code (true/false = down/up).
    // initialized with all falses.
    std::array<bool, 1024> m_keys;
};


