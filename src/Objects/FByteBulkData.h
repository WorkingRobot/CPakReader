#pragma once

#include "../CStream.h"
#include "../CPackageFileStream.h"
#include "EBulkDataFlags.h"

struct FByteBulkData {
    int64_t DataOffset;

	uint32_t BulkDataFlags;
    std::shared_ptr<char[]> Data;
    
    // https://github.com/EpicGames/UnrealEngine/blob/f8f4b403eb682ffc055613c7caf9d2ba5df7f319/Engine/Source/Runtime/CoreUObject/Public/Serialization/BulkData2.h#L327
    // https://github.com/EpicGames/UnrealEngine/blob/f8f4b403eb682ffc055613c7caf9d2ba5df7f319/Engine/Source/Runtime/CoreUObject/Public/Serialization/BulkData2.h#L249
    constexpr static uint8_t ElementSize = 1;

    friend CStream& operator>>(CStream& InputStream, FByteBulkData& ByteBulkData) {
        InputStream >> ByteBulkData.BulkDataFlags;

        int64_t ElementCount, BulkDataSizeOnDisk, BulkDataSize, BulkDataOffsetInFile;
        if (ByteBulkData.BulkDataFlags & BULKDATA_Size64Bit) {
            InputStream >> ElementCount;
            InputStream >> BulkDataSizeOnDisk;
        }
        else {
            int32_t Temp32ByteValue;

            InputStream >> Temp32ByteValue;
            ElementCount = Temp32ByteValue;

            InputStream >> Temp32ByteValue;
            BulkDataSizeOnDisk = Temp32ByteValue;
        }

        BulkDataSize = ElementCount * ElementSize;

        InputStream >> BulkDataOffsetInFile;

        if (ByteBulkData.BulkDataFlags & BULKDATA_BadDataVersion) {
            uint16_t DummyValue;
            InputStream >> DummyValue;
        }

        if (ByteBulkData.BulkDataFlags & BULKDATA_Unused) {
            return InputStream;
        }

        // Most of this code below is based off of FModel, so I'm just trusting that
        ByteBulkData.Data = std::shared_ptr<char[]>(new char[BulkDataSize]);

        if (ByteBulkData.BulkDataFlags & BULKDATA_OptionalPayload) { // uptnl
            auto ParentPackage = (CPackage*)InputStream.GetParentPackage();
            if (ParentPackage) {
                auto PtnlFile = ParentPackage->TryGetFile("uptnl");
                if (PtnlFile) {
                    CPackageFileStream PtnlStream(*PtnlFile);
                    PtnlStream.seek(BulkDataOffsetInFile + ByteBulkData.DataOffset, CStream::Begin);
                    PtnlStream.read(ByteBulkData.Data.get(), BulkDataSize);
                    return InputStream;
                }
            }
        }

        if (ByteBulkData.BulkDataFlags & BULKDATA_PayloadInSeperateFile) { // ubulk
            auto ParentPackage = (CPackage*)InputStream.GetParentPackage();
            if (ParentPackage) {
                auto BulkFile = ParentPackage->TryGetFile("ubulk");
                if (BulkFile) {
                    CPackageFileStream BulkStream(*BulkFile);
                    BulkStream.seek(BulkDataOffsetInFile + ByteBulkData.DataOffset, CStream::Begin);
                    BulkStream.read(ByteBulkData.Data.get(), BulkDataSize);
                    return InputStream;
                }
            }
        }

        if (ByteBulkData.BulkDataFlags & BULKDATA_ForceInlinePayload) { // uexp
            InputStream.read(ByteBulkData.Data.get(), BulkDataSize);
            return InputStream;
        }

        if (ByteBulkData.BulkDataFlags & BULKDATA_PayloadAtEndOfFile) { // uasset (?)
            auto OriginalOffset = InputStream.tell();
            InputStream.seek(BulkDataOffsetInFile, CStream::Begin);
            InputStream.read(ByteBulkData.Data.get(), BulkDataSize);
            InputStream.seek(OriginalOffset, CStream::Begin);
            return InputStream;
        }

        return InputStream;
    }
};