#pragma once

#include "../CStream.h"

struct FIntPoint {
    int X;
    int Y;

    friend CStream& operator>>(CStream& InputStream, FIntPoint& IntPoint) {
        InputStream >> IntPoint.X;
        InputStream >> IntPoint.Y;

        return InputStream;
    }
};