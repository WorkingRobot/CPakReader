#pragma once

#include "../Exports/UObject.h"

struct PBool {
	bool Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PBool Create(CStream& InputStream, FPropertyTag& Tag) {
		switch (ReadType)
		{
		case EReadType::NORMAL:
		{
			return { Tag.BoolVal != 0 };
		}
		case EReadType::MAP:
		case EReadType::ARRAY:
		{
			uint8_t Val;
			InputStream >> Val;
			return { Val != 0 };
		}
		}
	}
};
