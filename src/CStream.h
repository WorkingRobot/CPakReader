#pragma once

#include "HStringHash.h"

#include <codecvt>
#include <cstdint>
#include <stack>
#include <string>
#include <vector>

#define LOG_ERR(n, ...) printf(n "\n", __VA_ARGS__)
#define LOG_WRN(n, ...) printf(n "\n", __VA_ARGS__)

class CStream {
public:
    enum SeekPosition : int8_t {
        Begin,
        Cur,
        End
    };

    virtual CStream& read(char* Buf, size_t BufCount) = 0;
    virtual CStream& seek(size_t Position, SeekPosition SeekFrom) = 0;
    virtual size_t tell() = 0;
    virtual size_t size() = 0;

    CStream& operator>>(bool& Val) {
        int Val32;
        *this >> Val32;
        Val = Val32;
        return *this;
    }

    CStream& operator>>(int8_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(int16_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(int32_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(int64_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(uint8_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(uint16_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(uint32_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(uint64_t& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(float& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(double& Val) {
        read((char*)&Val, sizeof(Val));
        return *this;
    }

    CStream& operator>>(std::string& Val) {
        // > 0 for ANSICHAR, < 0 for UCS2CHAR serialization
        int SaveNum;
        *this >> SaveNum;

        bool LoadUCS2Char = SaveNum < 0;

        if (LoadUCS2Char)
        {
            // If SaveNum cannot be negated due to integer overflow, Ar is corrupted.
            if (SaveNum == INT_MIN)
            {
                return *this; // archive corrupted
            }

            SaveNum = -SaveNum;
        }

        if (SaveNum == 0)
        {
            Val.clear();
            return *this; // blank
        }

        if (LoadUCS2Char)
        {
            static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> ucs2conv;

            std::unique_ptr<char16_t[]> StringData = std::make_unique<char16_t[]>(SaveNum);
            read((char*)StringData.get(), SaveNum * sizeof(char16_t));
            Val = ucs2conv.to_bytes(StringData.get());
        }
        else {
            std::unique_ptr<char[]> StringData = std::make_unique<char[]>(SaveNum);
            read(StringData.get(), SaveNum);
            Val = StringData.get();
        }

        return *this;
    }

    // TArrays, usually
    template<class T>
    CStream& operator>>(std::vector<T>& Val) {
        int SerializeNum;
        *this >> SerializeNum;
        Val.reserve(SerializeNum);
        for (int i = 0; i < SerializeNum; ++i) {
            auto& Item = Val.emplace_back();
            *this >> Item;
        }
        return *this;
    }

    // Helps with TMaps that are std::vector<std::pair<K, V>>
    template<class K, class V>
    CStream& operator>>(std::pair<K, V>& Val) {
        *this >> Val.first;
        *this >> Val.second;
        return *this;
    }

    constexpr static uint32_t PropStructFallback = HStringHash::Crc32("StructFallback");
    constexpr static uint32_t PropDataOffset     = HStringHash::Crc32("DataOffset");
    constexpr static uint32_t PropParentPackage  = HStringHash::Crc32("ParentPackage");

    const std::stack<size_t>* TryGetProperty(size_t Key) {
        auto& ret = GetPropertyItr(Key);
        if (ret != Properties.end()) {
            return &ret->second;
        }
        return nullptr;
    }

    const size_t& GetProperty(uint32_t Key, size_t Default) {
        auto ret = TryGetProperty(Key);
        if (ret) {
            if (!ret->empty()) {
                return ret->top();
            }
            else {
                return Default;
            }
        }
        return Default;
    }

    void PushProperty(uint32_t Key, size_t Value) {
        auto& prop = GetPropertyItr(Key);
        if (prop != Properties.end()) {
            prop->second.push(Value);
        }
        else {
            Properties.emplace_back(Key, std::deque<size_t> { Value });
        }
    }

    bool PopProperty(uint32_t Key) {
        auto& prop = GetPropertyItr(Key);
        if (prop != Properties.end()) {
            prop->second.pop();
            return true;
        }
        return false;
    }

private:
    std::vector<std::pair<uint32_t, std::stack<size_t>>> Properties;

    decltype(Properties)::iterator GetPropertyItr(uint32_t Key) {
        return std::find_if(Properties.begin(), Properties.end(), [Key](const std::pair<uint32_t, std::stack<size_t>>& Pair) {
            return Pair.first == Key;
        });
    }
};