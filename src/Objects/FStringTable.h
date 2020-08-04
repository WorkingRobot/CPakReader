#pragma once

#include "../CStream.h"
#include "../Objects/FName.h"

struct FStringTable {
    // The namespace to use for all the strings in this table
    std::string TableNamespace;
    std::vector<std::pair<std::string, std::string>> StringTable;
    // UE4 calls this a TMap<FString, FMetaDataMap>, and FMetaDataMap is a typedef to TMap<FName, FString>
    std::vector<std::pair<std::string, std::vector<std::pair<FName, std::string>>>> MetaData;

    friend CStream& operator>>(CStream& InputStream, FStringTable& StringTable) {
        // https://github.com/EpicGames/UnrealEngine/blob/c39458e354cc77dbfd9abddac3ba542ccad46b8f/Engine/Source/Runtime/Core/Private/Internationalization/StringTableCore.cpp#L290

        InputStream >> StringTable.TableNamespace;
        // Simplifying this because we don't need pointers and table entry stuff everywhere
        InputStream >> StringTable.StringTable;
        InputStream >> StringTable.MetaData;

        return InputStream;
    }
};