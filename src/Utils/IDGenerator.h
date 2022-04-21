#pragma once

#include <string>

// utility class that auto increments ids
class IDGenerator
{
private:
    unsigned m_Value = 0;

public:
    IDGenerator() = default;

    inline void SetLastValue(unsigned value) { m_Value = value; }

    [[nodiscard]] inline std::string GenerateID() { return std::to_string(++m_Value); }
};
