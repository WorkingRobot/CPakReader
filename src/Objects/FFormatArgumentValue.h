#pragma once

#include "../CStream.h"
#include "EFormatArgumentType.h"
#include "FFormatArgumentData.h"
#include "FText.h"

#include <variant>

struct FFormatArgumentValue {
    std::variant<double, float, int64_t, uint64_t, FText> Value;

    FFormatArgumentValue() = default;

    FFormatArgumentValue(FFormatArgumentData&& Data) {
        std::visit([this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                Value = (int64_t)arg;
            else if constexpr (std::is_same_v<T, float>)
                Value = arg;
            else if constexpr (std::is_same_v<T, ETextGender>)
                Value = (uint64_t)arg;
            else if constexpr (std::is_same_v<T, FText>)
                Value = arg;
            else
                static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }, Data.ArgumentValue);
    }

    friend CStream& operator>>(CStream& InputStream, FFormatArgumentValue& FormatArgumentValue) {
        int8_t TypeAsInt8;
        InputStream >> TypeAsInt8;

        // https://github.com/EpicGames/UnrealEngine/blob/master/Engine/Source/Runtime/Core/Private/Internationalization/Text.cpp#L1210
        // EFormatArgumentType has a gender type, but it's not serializable? Yet, it has a GetGenderValue function??
        switch ((EFormatArgumentType)TypeAsInt8)
        {
        case Int:
        {
            int64_t Val;
            InputStream >> Val;
            FormatArgumentValue.Value = Val;
            break;
        }
        case UInt:
        {
            uint64_t Val;
            InputStream >> Val;
            FormatArgumentValue.Value = Val;
            break;
        }
        case Float:
        {
            float Val;
            InputStream >> Val;
            FormatArgumentValue.Value = Val;
            break;
        }
        case Double:
        {
            double Val;
            InputStream >> Val;
            FormatArgumentValue.Value = Val;
            break;
        }
        case Text:
        {
            FText Val;
            InputStream >> Val;
            FormatArgumentValue.Value = Val;
            break;
        }
        }

        return InputStream;
    }
};