#pragma once

#include <string>

// utility class that auto increments ids
class ID
{
private:
    static unsigned s_LastId;
    unsigned m_Value;

public:
    ID() : m_Value(s_LastId + 1) { ++s_LastId; }

    static void SetLastID(unsigned id) { s_LastId = id; }

    [[nodiscard]] inline std::string ToString() const { return std::to_string(m_Value); }
};

unsigned ID::s_LastId = 0;