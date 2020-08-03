#pragma once

#include "../CStream.h"
#include "../HStringHash.h"
#include "FName.h"
#include "FGuid.h"

struct FPropertyTag {
	int ArrayIndex;
	uint8_t BoolVal;
	FName EnumName;
	uint8_t HasPropertyGuid;
	FName InnerType;

	FName Name;

	FGuid PropertyGuid;
	int Size;
	FGuid StructGuid;
	FName StructName;
	FName Type;
	FName ValueType;

	friend CStream& operator>>(CStream& InputStream, FPropertyTag& PropertyTag) {
		InputStream >> PropertyTag.Name;

		if (PropertyTag.Name.IsNull() || PropertyTag.Name.IsNone()) {
			return InputStream;
		}

		InputStream >> PropertyTag.Type;
		InputStream >> PropertyTag.Size;
		InputStream >> PropertyTag.ArrayIndex;

		if (PropertyTag.Type.Number == 0)
		{
			switch (HStringHash::Crc32RT(PropertyTag.Type.c_str()))
			{
			case HStringHash::Crc32("StructProperty"):
				InputStream >> PropertyTag.StructName;
				// FileVersionUE4 >= VER_UE4_STRUCT_GUID_IN_PROPERTY_TAG
				InputStream >> PropertyTag.StructGuid;
				break;
			case HStringHash::Crc32("BoolProperty"):
				InputStream >> PropertyTag.BoolVal;
				break;
			case HStringHash::Crc32("ByteProperty"):
			case HStringHash::Crc32("EnumProperty"):
				InputStream >> PropertyTag.EnumName;
				break;
			case HStringHash::Crc32("ArrayProperty"):
				// FileVersionUE4 >= VAR_UE4_ARRAY_PROPERTY_INNER_TAGS
				InputStream >> PropertyTag.InnerType;
				break;
			case HStringHash::Crc32("SetProperty"):
				// FileVersionUE4 >= VER_UE4_PROPERTY_TAG_SET_MAP_SUPPORT
				InputStream >> PropertyTag.InnerType;
				break;
			case HStringHash::Crc32("MapProperty"):
				InputStream >> PropertyTag.InnerType;
				InputStream >> PropertyTag.ValueType;
				break;
			}
		}

		// FileVersionUE4 >= VER_UE4_PROPERTY_GUID_IN_PROPERTY_TAG
		// Property tags to handle renamed blueprint properties effectively.
		InputStream >> PropertyTag.HasPropertyGuid;
		if (PropertyTag.HasPropertyGuid)
		{
			InputStream >> PropertyTag.PropertyGuid;
		}

		return InputStream;
	}
};
