#pragma once

#include "../CStream.h"
#include "ERoundingMode.h"

struct FNumberFormattingOptions {
    bool AlwaysSign;
    bool UseGrouping;
    ERoundingMode RoundingMode;
    int MinimumIntegralDigits;
    int MaximumIntegralDigits;
    int MinimumFractionalDigits;
    int MaximumFractionalDigits;

    friend CStream& operator>>(CStream& InputStream, FNumberFormattingOptions& NumberFormattingOptions) {
        InputStream >> NumberFormattingOptions.AlwaysSign;
        InputStream >> NumberFormattingOptions.UseGrouping;

        int8_t RoundingModeInt8;
        InputStream >> RoundingModeInt8;
        NumberFormattingOptions.RoundingMode = (ERoundingMode)RoundingModeInt8;

        InputStream >> NumberFormattingOptions.MinimumIntegralDigits;
        InputStream >> NumberFormattingOptions.MaximumIntegralDigits;
        InputStream >> NumberFormattingOptions.MinimumFractionalDigits;
        InputStream >> NumberFormattingOptions.MaximumFractionalDigits;

        return InputStream;
    }
};