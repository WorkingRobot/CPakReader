#pragma once

#include <cstdint>

enum EDateTimeStyle : uint8_t
{
    Default,
    Short,
    Medium,
    Long,

    Full
    // Add new enum types at the end only! They are serialized by index.
};
