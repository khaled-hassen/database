#pragma once

#include <string>

// utility class that auto increments ids
class ID
{
private:
    static unsigned lastId;
    unsigned value;

public:
    ID() : value(lastId + 1) { ++lastId; }

    static void SetLastID(unsigned id) { lastId = id; }

    [[nodiscard]] std::string ToString() const { return std::to_string(value); }
};

unsigned ID::lastId = 0;