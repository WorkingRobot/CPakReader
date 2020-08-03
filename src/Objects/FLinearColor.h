#pragma once

#include "../CStream.h"

struct FLinearColor {
    float R;
    float G;
    float B;
    float A;

    friend CStream& operator>>(CStream& InputStream, FLinearColor& LinearColor) {
        InputStream >> LinearColor.R;
        InputStream >> LinearColor.G;
        InputStream >> LinearColor.B;
        InputStream >> LinearColor.A;

        return InputStream;
    }
};