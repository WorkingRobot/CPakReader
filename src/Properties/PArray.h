#pragma once

#include "../Exports/UObject.h"

#include <any>

struct PArray {
	std::vector<std::any> Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PArray Create(CStream& InputStream, FPropertyTag& Tag) {
		int SerializeNum;
		FPropertyTag InnerTag;

		InputStream >> SerializeNum;
		if (HStringHash::Crc32RT(Tag.InnerType.c_str()) == HStringHash::Crc32("StructProperty")) {
			InputStream >> InnerTag;
		}

		std::vector<std::any> Val;
		Val.reserve(SerializeNum);
		for (int i = 0; i < SerializeNum; ++i) {
			Val.emplace_back(UObject::ReadProperty<EReadType::ARRAY>(InputStream, InnerTag, Tag.InnerType));
		}

		return { Val };
	}
};
