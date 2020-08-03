#pragma once

#include "../CStream.h"

struct FQuat {
    float X;
    float Y;
    float Z;
    float W;

    friend CStream& operator>>(CStream& InputStream, FQuat& Quat) {
        InputStream >> Quat.X;
        InputStream >> Quat.Y;
        InputStream >> Quat.Z;
        InputStream >> Quat.W;

        return InputStream;
    }
};