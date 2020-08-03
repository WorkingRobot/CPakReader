#pragma once

#include "../CStream.h"
#include "EFormatArgumentType.h"
#include "ETextGender.h"
#include "FText.h"

#include <variant>

struct FFormatArgumentData {
    std::string ArgumentName;

    std::variant<FText, int, float, ETextGender> ArgumentValue;

    friend CStream& operator>>(CStream& InputStream, FFormatArgumentData& FormatArgumentData) {
        // https://github.com/EpicGames/UnrealEngine/blob/bb72dc4be632932e44f7e7eb7082c0e69a93562b/Engine/Source/Runtime/Core/Private/Internationalization/Text.cpp#L1410
        // "There was no version bump for this change, but VER_UE4_K2NODE_VAR_REFERENCEGUIDS was made at almost the same time."
        // UE4Ver >= VER_UE4_K2NODE_VAR_REFERENCEGUIDS
        InputStream >> FormatArgumentData.ArgumentName;

        uint8_t TypeAsByte;
        InputStream >> TypeAsByte;
        switch ((EFormatArgumentType)TypeAsByte)
        {
        case Int:
        {
            int Val;
            InputStream >> Val;
            FormatArgumentData.ArgumentValue = Val;
            break;
        }
        case Float:
        {
            float Val;
            InputStream >> Val;
            FormatArgumentData.ArgumentValue = Val;
            break;
        }
        case Text:
        {
            FText Val;
            InputStream >> Val;
            FormatArgumentData.ArgumentValue = Val;
            break;
        }
        case Gender:
        {
            uint8_t Val;
            InputStream >> Val;
            FormatArgumentData.ArgumentValue = (ETextGender)Val;
            break;
        }
        }

        return InputStream;
    }
};