#pragma once

#include "../CStream.h"

struct FDateTime {
    // Holds the ticks in 100 nanoseconds resolution since January 1, 0001 A.D.
    int64_t Ticks;

    friend CStream& operator>>(CStream& InputStream, FDateTime& DateTime) {
        InputStream >> DateTime.Ticks;

        return InputStream;
    }
};