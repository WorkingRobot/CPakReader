#pragma once

class CStream {
public:
    enum SeekPosition : int8_t {
        Begin,
        Cur,
        End
    };

    virtual CStream& read(char* Buf, size_t BufCount) = 0;
    virtual CStream& seek(size_t Position, SeekPosition SeekFrom) = 0;

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
};