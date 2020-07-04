#pragma once

#include "../EErrorCode.h"

#include <istream>

class RPakReader {
public:
	RPakReader(const std::istream& InputStream, EErrorCode& ErrorCode);
};