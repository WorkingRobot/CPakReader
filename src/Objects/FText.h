#pragma once

#include "../CStream.h"
#include "ETextFlag.h"
#include "ETextHistoryType.h"

#include <any>

struct FTextHistory;

struct FText {
    ETextFlag Flags;
    std::shared_ptr<FTextHistory> Text;

    friend CStream& operator>>(CStream& InputStream, FText& Text);
};