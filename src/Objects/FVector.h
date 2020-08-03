#pragma once

#include "../CStream.h"

struct FVector {
    float X;
    float Y;
    float Z;

    friend CStream& operator>>(CStream& InputStream, FVector& Vector) {
        InputStream >> Vector.X;
        InputStream >> Vector.Y;
        InputStream >> Vector.Z;

        return InputStream;
    }
};