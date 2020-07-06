#pragma once

#include <codecvt>
#include <cstdint>
#include <string>
#include <vector>

class CStream {
public:
    enum SeekPosition : int8_t {
        Begin,
        Cur,
        End
    };

    virtual CStream& read(char* Buf, size_t BufCount) = 0;
    virtual CStream& seek(size_t Position, SeekPosition SeekFrom) = 0;
    virtual size_t size() = 0;

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
};