#pragma once

#include <cstdint>

// https://stackoverflow.com/a/28801005

class HStringHash {
public:
    template<size_t size>
    static constexpr uint32_t Crc32(const char(&str)[size], bool insensitive = false) {
        return Crc32(str, size - 1, insensitive);
    }

    static constexpr uint32_t Crc32(const char* str, size_t size, bool insensitive = false) {
        return ~crc32_impl(str, size, ~0, insensitive);
    }

private:
    template <unsigned c, int k = 8>
    struct f : f<((c & 1) ? 0xedb88320 : 0) ^ (c >> 1), k - 1> {};
    template <unsigned c> struct f<c, 0> { enum { value = c }; };

    #define A(x) B(x) B(x + 128)
    #define B(x) C(x) C(x +  64)
    #define C(x) D(x) D(x +  32)
    #define D(x) E(x) E(x +  16)
    #define E(x) F(x) F(x +   8)
    #define F(x) G(x) G(x +   4)
    #define G(x) H(x) H(x +   2)
    #define H(x) I(x) I(x +   1)
    #define I(x) f<x>::value ,

    static constexpr int crc_table[] = { A(0) };

    #undef A
    #undef B
    #undef C
    #undef D
    #undef E
    #undef F
    #undef G
    #undef H
    #undef I

    static constexpr uint32_t crc32_impl(const char* p, size_t len, uint32_t crc, bool insensitive) {
        return len ?
            crc32_impl(p + 1, len - 1, (crc >> 8) ^ crc_table[(crc & 0xFF) ^ (!insensitive ? *p : (*p >= 'a' && *p <= 'z' ? *p - 0x20 : *p))], insensitive)
            : crc;
    }
};