#pragma once

#include "../CStream.h"
#include "../EReadType.h"
#include "../Objects/FPropertyTag.h"

#include <any>

struct UObject {
    std::vector<std::pair<std::string, std::any>> Properties;
    FGuid Guid;

    const std::any* GetProperty(const char* Key) const {
        auto& ret = std::find_if(Properties.begin(), Properties.end(), [Key](const std::pair<std::string, std::any>& Property) {
            return Property.first == Key;
        });
        if (ret != Properties.end()) {
            return &ret->second;
        }
        return nullptr;
    }

    template<class T>
    const T* GetProperty(const char* Key) const {
        auto ret = GetProperty(Key);
        if (ret) {
            return std::any_cast<T>(ret);
        }
        return nullptr;
    }

    friend CStream& operator>>(CStream& InputStream, UObject& Object);

    template<EReadType ReadType = EReadType::NORMAL>
    static std::any ReadProperty(CStream& InputStream, FPropertyTag& Tag, FName& Type);

private:
    void AddProperty(FPropertyTag& Tag, std::any& Property);
};