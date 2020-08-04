#pragma once

#include "../CStream.h"
#include "ETextFlag.h"

struct FTextHistory;

struct FText {
    ETextFlag Flags;
    std::shared_ptr<FTextHistory> Text;

    friend CStream& operator>>(CStream& InputStream, FText& Text);
};