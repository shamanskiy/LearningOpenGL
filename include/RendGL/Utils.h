#pragma once

#include <string>

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

    bool ok() { return m_ok; }
    const std::string& message() { return m_message; }

private:
    // Check first: true = ok, false = not ok, check message
    bool m_ok;
    // Message if false
    std::string m_message;
};
