#pragma once

#include <cstdint>
#include <type_traits>

class HAlign {
public:
    template<class T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    static constexpr T Align(T Value, T Alignment) {
        return Value + Alignment - 1 & ~(Alignment - 1);
    }
};