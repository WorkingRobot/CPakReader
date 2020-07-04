#pragma once

#include "../CStream.h"
#include "EPakVersion.h"
#include "FSHAHash.h"
#include "FGuid.h"

#include <cstdint>
#include <string>
#include <vector>

struct FPakInfo {
    static constexpr uint32_t FileMagic = 0x5A6F12E1;
    static constexpr int CompressionMethodNameLen = 32;
    static constexpr int CompressionMethodMaxCount = 5;

    uint32_t Magic;

    int32_t Version;

    int64_t IndexOffset;

    int64_t IndexSize;

    FSHAHash IndexHash;

    uint8_t bEncryptedIndex;

    FGuid EncryptionKeyGuid;

    std::vector<std::string> CompressionMethods;

    friend CStream& operator>>(CStream& InputStream, FPakInfo& PakInfo) {
        InputStream >> PakInfo.EncryptionKeyGuid;
        InputStream >> PakInfo.bEncryptedIndex;

        InputStream >> PakInfo.Magic;
        if (PakInfo.Magic != FPakInfo::FileMagic) {
            PakInfo.Magic = 0;
            return InputStream;
        }
        InputStream >> PakInfo.Version;
        InputStream >> PakInfo.IndexOffset;
        InputStream >> PakInfo.IndexSize;
        InputStream >> PakInfo.IndexHash;

        if (PakInfo.Version < PAKVER_INDEX_ENCRYPTION) {
            PakInfo.bEncryptedIndex = false;
        }
        if (PakInfo.Version < PAKVER_ENCRYPTION_KEY_GUID) {
            PakInfo.EncryptionKeyGuid.Invalidate();
        }

        if (PakInfo.Version >= PAKVER_FROZEN_INDEX && PakInfo.Version < PAKVER_PATH_HASH_INDEX) {
            uint8_t bIndexIsFrozen;
            InputStream >> bIndexIsFrozen;
            // https://github.com/EpicGames/UnrealEngine/blob/0d7ca47c39ae27a06116bf57b413f44df15d8179/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L218
            if (bIndexIsFrozen) {
                PakInfo.Magic = 0;
                return InputStream;
            }
        }

        if (PakInfo.Version < PAKVER_FNAME_BASED_COMPRESSION_METHOD) {
            PakInfo.CompressionMethods.reserve(4);
            PakInfo.CompressionMethods.emplace_back("None");
            PakInfo.CompressionMethods.emplace_back("Zlib");
            PakInfo.CompressionMethods.emplace_back("Gzip");
            PakInfo.CompressionMethods.emplace_back("Oodle");
        }
        else {
            PakInfo.CompressionMethods.emplace_back("None");

            constexpr int BufferSize = FPakInfo::CompressionMethodNameLen * FPakInfo::CompressionMethodMaxCount;
            char Methods[BufferSize];
            InputStream.read(Methods, BufferSize);
            for (int i = 0; i < BufferSize; i += FPakInfo::CompressionMethodNameLen) {
                char* MethodString = Methods + i;
                if (*MethodString) {
                    PakInfo.CompressionMethods.emplace_back(MethodString);
                }
            }
        }

        return InputStream;
    }
};