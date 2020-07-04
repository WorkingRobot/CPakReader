#pragma once

#include "CPackage.h"

#include <unordered_map>
#include <variant>

class CFileTree {

	std::unordered_map<std::unique_ptr<char[]>, std::variant<CFileTree, CPackage>> Children;
};