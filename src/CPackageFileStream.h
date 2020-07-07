#pragma once

#include "ECompressionMethod.h"
#include "CMemoryStream.h"
#include "CPackageFile.h"
#include "CStream.h"
#include "HAES.h"
#include "HAlign.h"

#include <libdeflate.h>

class CPackageFileStream : public CStream {
public:
    CPackageFileStream(const CPackage& Package, const char* Extension) : CPackageFileStream(((CPackage&)Package).GetFile(Extension)) {}

    CPackageFileStream(const CPackageFile& PackageFile) : PackageFile(PackageFile) {
        CStream* Stream = PackageFile.PakFile.Stream;

        if (!PackageFile.CompInfo) { // no compression
            Stream->seek(PackageFile.Offset + PackageFile.StructSize, CStream::Begin);

            if (PackageFile.Encrypted) {
                int BufferSize = HAlign::Align(PackageFile.Size, 16);
                Buffer.reset(new char[BufferSize]); // 16 = FAES::AESBlockSize
                Stream->read(Buffer.get(), BufferSize);
                HAES::DecodeInPlace(PackageFile.PakFile.Key, Buffer.get(), BufferSize);
            }
            else {
                Buffer.reset(new char[PackageFile.Size]);
                Stream->read(Buffer.get(), PackageFile.Size);
            }
            MemoryStream = std::make_unique<CMemoryStream>(Buffer, PackageFile.Size);
        }
        else {
            // TODO: move this method result into the CPackageFile to save space and compute time when reading
            auto CompressionMethod = ECompressionMethodHelper::GetType(PackageFile.PakFile.Info.CompressionMethods[PackageFile.CompInfo->CompressionMethodIndex].c_str());
            if (CompressionMethod == ECompressionMethod::UNKNOWN) {
                // unknown method
                return;
            }

            int64_t BytesRead = 0;
            Buffer.reset(new char[PackageFile.CompInfo->UncompressedSize]); // 16 = FAES::AESBlockSize

            for (auto& CompressionBlock : PackageFile.CompInfo->CompressionBlocks) {
                Stream->seek(PackageFile.Offset + CompressionBlock.CompressedStart, CStream::Begin);

                uint32_t UncompressedSize = std::min((int64_t)PackageFile.CompInfo->CompressionBlockSize, PackageFile.CompInfo->UncompressedSize - BytesRead);

                std::unique_ptr<char[]> BlockBuffer;

                if (PackageFile.Encrypted) {
                    int BlockBufferSize = HAlign::Align(CompressionBlock.CompressedEnd - CompressionBlock.CompressedStart, (uint64_t)16);
                    BlockBuffer = std::make_unique<char[]>(BlockBufferSize); // 16 = FAES::AESBlockSize
                    Stream->read(BlockBuffer.get(), BlockBufferSize);
                    HAES::DecodeInPlace(PackageFile.PakFile.Key, BlockBuffer.get(), BlockBufferSize);
                }
                else {
                    int BlockBufferSize = CompressionBlock.CompressedEnd - CompressionBlock.CompressedStart;
                    BlockBuffer = std::make_unique<char[]>(BlockBufferSize);
                    Stream->read(BlockBuffer.get(), BlockBufferSize);
                }

                switch (CompressionMethod)
                {
                case ECompressionMethod::ZLIB:
                {
                    libdeflate_decompressor* Decompressor = libdeflate_alloc_decompressor();
                    libdeflate_result Result = libdeflate_zlib_decompress(Decompressor, BlockBuffer.get(), CompressionBlock.CompressedEnd - CompressionBlock.CompressedStart, Buffer.get() + BytesRead, UncompressedSize, NULL);
                    if (Result != LIBDEFLATE_SUCCESS) {
                        // didn't decompress it all
                    }
                    libdeflate_free_decompressor(Decompressor);
                    break;
                }
                case ECompressionMethod::GZIP:
                {
                    libdeflate_decompressor* Decompressor = libdeflate_alloc_decompressor();
                    libdeflate_result Result = libdeflate_gzip_decompress(Decompressor, BlockBuffer.get(), CompressionBlock.CompressedEnd - CompressionBlock.CompressedStart, Buffer.get() + BytesRead, UncompressedSize, NULL);
                    if (Result != LIBDEFLATE_SUCCESS) {
                        // didn't decompress it all
                    }
                    libdeflate_free_decompressor(Decompressor);
                    break;
                }
                case ECompressionMethod::OODLE:
                    // TODO: implement oodle decompression
                    break;
                default:
                    break;
                }
            }

            MemoryStream = std::make_unique<CMemoryStream>(Buffer, PackageFile.CompInfo->UncompressedSize);
        }
    }

    CStream& read(char* Buf, size_t BufCount) override {
        MemoryStream->read(Buf, BufCount);
        return *this;
    }

    CStream& seek(size_t Position, SeekPosition SeekFrom) override {
        MemoryStream->seek(Position, SeekFrom);
        return *this;
    }

    size_t size() override {
        return MemoryStream->size();
    }

private:
    const CPackageFile& PackageFile;
    std::unique_ptr<CMemoryStream> MemoryStream;
    std::shared_ptr<char[]> Buffer;
};