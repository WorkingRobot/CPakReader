#pragma once

#include "../CStream.h"

struct FGameplayTagContainer {
    // UE4 has this as TArray<FGameplayTag>, but FGameplayTag is just a fancy FName
    std::vector<FName> GameplayTags;

    friend CStream& operator>>(CStream& InputStream, FGameplayTagContainer& GameplayTagContainer) {
        InputStream >> GameplayTagContainer.GameplayTags;

        return InputStream;
    }
};