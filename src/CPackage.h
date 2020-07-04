#pragma once

#include "EAssetType.h"

#include <memory>
#include <unordered_map>
#include <variant>

class CPackage {

	std::unordered_map<std::unique_ptr<char[]>, std::variant<EAssetType, std::unique_ptr<char[]>>> Files;
};