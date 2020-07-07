#pragma once

#include "../CStream.h"
#include "ECompressionFlags.h"
#include "EPackageFlags.h"
#include "FCompressedChunk.h"
#include "FCustomVersionContainer.h"
#include "FEngineVersion.h"
#include "FGenerationInfo.h"

#include <cstdint>

struct FPackageFileSummary {
    static constexpr uint32_t PackageFileTag = 0x9E2A83C1;
    static constexpr uint32_t PackageFileTagSwapped = 0xC1832A9E;

    // This enum is huge, so we're not really going to do any version checking
    // https://github.com/EpicGames/UnrealEngine/blob/f8f4b403eb682ffc055613c7caf9d2ba5df7f319/Engine/Source/Runtime/Core/Public/UObject/ObjectVersion.h#L16
    int FileVersionUE4;
    int FileVersionLicenseeUE4;
    FCustomVersionContainer CustomVersionContainer;

    int TotalHeaderSize;
    std::string FolderName;
    EPackageFlags PackageFlags;
    int NameCount;
    int NameOffset;

    // FileVersionUE4 >= VER_UE4_SERIALIZE_TEXT_IN_PACKAGES
    int GatherableTextDataCount;
    int GatherableTextDataOffset;
    int ExportCount;
    int ExportOffset;
    int ImportCount;
    int ImportOffset;
    int DependsOffset;

    // FileVersionUE4 >= VER_UE4_ADD_STRING_ASSET_REFERENCES_MAP
    int SoftPackageReferencesCount;
    int SoftPackageReferencesOffset;

    // FileVersionUE4 >= VER_UE4_ADDED_SEARCHABLE_NAMES
    int SearchableNamesOffset;
    int ThumbnailTableOffset;
    FGuid Guid;
    std::vector<FGenerationInfo> Generations;

    // FileVersionUE4 >= VER_UE4_ENGINE_VERSION_OBJECT
    FEngineVersion SavedByEngineVersion;

    // FileVersionUE4 >= VER_UE4_PACKAGE_SUMMARY_HAS_COMPATIBLE_ENGINE_VERSION
    FEngineVersion CompatibleWithEngineVersion;
    ECompressionFlags CompressionFlags;
    uint32_t PackageSource;
    std::vector<std::string> AdditionalPackagesToCook;
    int AssetRegistryDataOffset;
    int64_t BulkDataStartOffset;

    // FileVersionUE4 >= VER_UE4_WORLD_LEVEL_INFO
    int WorldTileInfoDataOffset;

    // FileVersionUE4 >= VER_UE4_CHANGED_CHUNKID_TO_BE_AN_ARRAY_OF_CHUNKIDS
    std::vector<int> ChunkIDs;

    // FileVersionUE4 >= VER_UE4_PRELOAD_DEPENDENCIES_IN_COOKED_EXPORTS
    int PreloadDependencyCount;
    int PreloadDependencyOffset;

    friend CStream& operator>>(CStream& InputStream, FPackageFileSummary& PackageFileSummary) {
        uint32_t Tag;
        InputStream >> Tag;

        if (Tag != PackageFileTag && Tag != PackageFileTagSwapped) {
            // Not a UE package
            return InputStream;
        }

        if (Tag == PackageFileTagSwapped) {
            // Endianness is swapped, we don't support that
            return InputStream;
        }

        int LegacyFileVersion;
        InputStream >> LegacyFileVersion;

        if (LegacyFileVersion < 0) {
            if (LegacyFileVersion < -7) { // CurrentLegacyFileVersion
                // UE3 file lmao
                return InputStream;
            }

            if (LegacyFileVersion != -4)
            {
                InputStream.seek(4, CStream::Cur); // LegacyUE3Version (int32)
            }

            // If these are both zero, it's "unversioned"
            InputStream >> PackageFileSummary.FileVersionUE4;
            InputStream >> PackageFileSummary.FileVersionLicenseeUE4;

            if (LegacyFileVersion <= -2)
            {
                InputStream >> PackageFileSummary.CustomVersionContainer;
            }
        }
        else {
            // UE3 file lmao
            return InputStream;
        }

        InputStream >> PackageFileSummary.TotalHeaderSize;
        InputStream >> PackageFileSummary.FolderName;
        uint32_t PackageFlags;
        InputStream >> PackageFlags;
        PackageFileSummary.PackageFlags = (EPackageFlags)PackageFlags;
        InputStream >> PackageFileSummary.NameCount;
        InputStream >> PackageFileSummary.NameOffset;

        // FileVersionUE4 >= VER_UE4_SERIALIZE_TEXT_IN_PACKAGES
        InputStream >> PackageFileSummary.GatherableTextDataCount;
        InputStream >> PackageFileSummary.GatherableTextDataOffset;
        InputStream >> PackageFileSummary.ExportCount;
        InputStream >> PackageFileSummary.ExportOffset;
        InputStream >> PackageFileSummary.ImportCount;
        InputStream >> PackageFileSummary.ImportOffset;
        InputStream >> PackageFileSummary.DependsOffset;

        // FileVersionUE4 >= VER_UE4_ADD_STRING_ASSET_REFERENCES_MAP
        InputStream >> PackageFileSummary.SoftPackageReferencesCount;
        InputStream >> PackageFileSummary.SoftPackageReferencesOffset;

        // FileVersionUE4 >= VER_UE4_ADDED_SEARCHABLE_NAMES
        InputStream >> PackageFileSummary.SearchableNamesOffset;
        InputStream >> PackageFileSummary.ThumbnailTableOffset;
        InputStream >> PackageFileSummary.Guid;
        InputStream >> PackageFileSummary.Generations;

        // FileVersionUE4 >= VER_UE4_ENGINE_VERSION_OBJECT
        InputStream >> PackageFileSummary.SavedByEngineVersion;

        // FileVersionUE4 >= VER_UE4_PACKAGE_SUMMARY_HAS_COMPATIBLE_ENGINE_VERSION
        InputStream >> PackageFileSummary.CompatibleWithEngineVersion;
        uint32_t CompressionFlags;
        InputStream >> CompressionFlags;
        PackageFileSummary.CompressionFlags = (ECompressionFlags)CompressionFlags;
        if (PackageFileSummary.CompressionFlags != ECompressionFlags::COMPRESS_None) {
            // deprecated compression
            return InputStream;
        }

        std::vector<FCompressedChunk> CompressedChunks;
        InputStream >> CompressedChunks;
        if (CompressedChunks.size()) {
            // package level compression isn't supported
            return InputStream;
        }

        InputStream >> PackageFileSummary.PackageSource;
        InputStream >> PackageFileSummary.AdditionalPackagesToCook;

        if (LegacyFileVersion > -7) {
            int NumTextureAllocations;
            InputStream >> NumTextureAllocations;
            if (NumTextureAllocations) {
                // UE3 file lmao
                return InputStream;
            }
        }

        InputStream >> PackageFileSummary.AssetRegistryDataOffset;
        InputStream >> PackageFileSummary.BulkDataStartOffset;

        // FileVersionUE4 >= VER_UE4_WORLD_LEVEL_INFO
        InputStream >> PackageFileSummary.WorldTileInfoDataOffset;

        // FileVersionUE4 >= VER_UE4_CHANGED_CHUNKID_TO_BE_AN_ARRAY_OF_CHUNKIDS
        InputStream >> PackageFileSummary.ChunkIDs;

        // FileVersionUE4 >= VER_UE4_PRELOAD_DEPENDENCIES_IN_COOKED_EXPORTS
        InputStream >> PackageFileSummary.PreloadDependencyCount;
        InputStream >> PackageFileSummary.PreloadDependencyOffset;

        return InputStream;
    }
};