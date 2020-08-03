#pragma once

#include "../CStream.h"

struct FRotator {
    float Pitch;
    float Yaw;
    float Roll;

    friend CStream& operator>>(CStream& InputStream, FRotator& Rotator) {
        InputStream >> Rotator.Pitch;
        InputStream >> Rotator.Yaw;
        InputStream >> Rotator.Roll;

        return InputStream;
    }
};