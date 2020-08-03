#pragma once

#include "../CStream.h"
#include "../EReadType.h"
#include "../Objects/FPropertyTag.h"

#include <any>

struct UObject {
    bool StructFallback = false;

    std::vector<std::pair<std::string, std::any>> Properties;
    FGuid Guid;

    friend CStream& operator>>(CStream& InputStream, UObject& Object);

    template<EReadType ReadType = EReadType::NORMAL>
    static std::any ReadProperty(CStream& InputStream, FPropertyTag& Tag, FName& Type);

private:
    void AddProperty(FPropertyTag& Tag, std::any& Property);
};