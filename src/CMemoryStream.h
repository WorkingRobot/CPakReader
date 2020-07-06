#pragma once

#include "CStream.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class CMemoryStream : public CStream {
public:
    CMemoryStream(const std::shared_ptr<char[]>& Buffer, size_t BufferSize) : Buffer(Buffer), Size(BufferSize), Position(0) {}

    CStream& read(char* Buf, size_t BufCount) override {
        memcpy(Buf, Buffer.get() + Position, BufCount);
        Position += BufCount;
        return *this;
    }

    CStream& seek(size_t Position, SeekPosition SeekFrom) override {\
        switch (SeekFrom)
        {
        case CStream::Begin:
            this->Position = Position;
            break;
        case CStream::Cur:
            this->Position += Position;
            break;
        case CStream::End:
            this->Position = Size - Position;
            break;
        }
        return *this;
    }

    size_t size() override {
        return Size;
    }

private:
    std::shared_ptr<char[]> Buffer;
    size_t Size;
    size_t Position;
};