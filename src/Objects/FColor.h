#pragma once

#include "../CStream.h"

struct FColor {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;

    friend CStream& operator>>(CStream& InputStream, FColor& Color) {
        InputStream >> Color.R;
        InputStream >> Color.G;
        InputStream >> Color.B;
        InputStream >> Color.A;

        return InputStream;
    }
};