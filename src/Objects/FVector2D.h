#pragma once

#include "../CStream.h"

struct FVector2D {
    float X;
    float Y;

    friend CStream& operator>>(CStream& InputStream, FVector2D& Vector2D) {
        InputStream >> Vector2D.X;
        InputStream >> Vector2D.Y;

        return InputStream;
    }
};