#pragma once

#include "CStream.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

class CFileStream : public CStream {
public:
    CFileStream(const fs::path& FilePath) : FileStream(FilePath, std::ios::in | std::ios::binary), FileSize(fs::file_size(FilePath)) {}

    CFileStream(const char* FilePath) : FileStream(FilePath, std::ios::in | std::ios::binary), FileSize(fs::file_size(FilePath)) {}

    CStream& read(char* Buf, size_t BufCount) override {
        FileStream.read(Buf, BufCount);
        return *this;
    }

    CStream& seek(size_t Position, SeekPosition SeekFrom) override {
        switch (SeekFrom)
        {
        case CStream::Begin:
            FileStream.seekg(Position, std::ios::beg);
            break;
        case CStream::Cur:
            FileStream.seekg(Position, std::ios::cur);
            break;
        case CStream::End:
            FileStream.seekg(Position, std::ios::end);
            break;
        }
        return *this;
    }

    size_t size() override {
        return FileSize;
    }

private:
    std::ifstream FileStream;
    size_t FileSize;
};