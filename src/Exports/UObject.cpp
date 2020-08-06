#include "UObject.h"

#include "../Readers/RAssetReader.h"
#include "../Properties/PByte.h"
#include "../Properties/PBool.h"
#include "../Properties/PInt.h"
#include "../Properties/PFloat.h"
#include "../Properties/PObject.h"
#include "../Properties/PName.h"
#include "../Properties/PDelegate.h"
#include "../Properties/PDouble.h"
#include "../Properties/PArray.h"
#include "../Properties/PStruct.h"
#include "../Properties/PStr.h"
#include "../Properties/PText.h"
#include "../Properties/PInterface.h"
#include "../Properties/PMulticastDelegate.h"
#include "../Properties/PLazyObject.h"
#include "../Properties/PSoftObjectMap.h"
#include "../Properties/PSoftObject.h"
#include "../Properties/PUInt64.h"
#include "../Properties/PUInt32.h"
#include "../Properties/PUInt16.h"
#include "../Properties/PInt64.h"
#include "../Properties/PInt16.h"
#include "../Properties/PInt8.h"
#include "../Properties/PMap.h"
#include "../Properties/PSet.h"
#include "../Properties/PEnum.h"

CStream& operator>>(CStream& InputStream, UObject& Object) {
	while (true) {
		FPropertyTag Tag;
		InputStream >> Tag;

		if (Tag.Name.IsNull() || Tag.Name.IsNone()) {
			break;
		}

		auto StartingPosition = InputStream.tell();

		Object.AddProperty(Tag, Object.ReadProperty(InputStream, Tag, Tag.Type));

		if (StartingPosition + Tag.Size != InputStream.tell()) {
			LOG_ERR("Didn't read %s (%s) correctly (at %d, should be %d, %d behind)", Tag.Type.c_str(), Tag.Name.c_str(), InputStream.tell(), StartingPosition + Tag.Size, StartingPosition + Tag.Size - InputStream.tell());
			InputStream.seek(StartingPosition + Tag.Size, CStream::Begin);
		}
	}

	if (!InputStream.GetProperty(CStream::PropStructFallback, false)) {
		bool HasStructFallbackGuid;
		InputStream >> HasStructFallbackGuid;
		if (HasStructFallbackGuid) {
			InputStream >> Object.Guid;
		}
	}

	return InputStream;
}

template<EReadType ReadType>
std::any UObject::ReadProperty(CStream& InputStream, FPropertyTag& Tag, FName& Type)
{
	// https://github.com/EpicGames/UnrealEngine/blob/f8f4b403eb682ffc055613c7caf9d2ba5df7f319/Engine/Source/Runtime/Core/Public/UObject/UnrealNames.inl#L
	switch (HStringHash::Crc32RT(Type.c_str()))
	{
#define CASE_STR(n) #n
#define CASE(t) case HStringHash::Crc32(CASE_STR(t##Property)): return P##t::Create<ReadType>(InputStream, Tag);

		CASE(Byte);
		CASE(Bool);
		CASE(Int);
		CASE(Float);
		CASE(Object);
		CASE(Name);
		CASE(Delegate);
		CASE(Double);
		CASE(Array);
		CASE(Struct);
		// No code in UE4 source despite these being technically serializable properties
		// CASE(Vector);
		// CASE(Rotator);
		CASE(Str);
		CASE(Text);
		CASE(MulticastDelegate);
		CASE(LazyObject);
		case HStringHash::Crc32("SoftObjectProperty"):
			if constexpr (ReadType == EReadType::MAP) {
				return PSoftObjectMap::Create<ReadType>(InputStream, Tag);
			}
			else {
				return PSoftObject::Create<ReadType>(InputStream, Tag);
			}
		CASE(UInt64);
		CASE(UInt32);
		CASE(UInt16);
		CASE(Int16);
		CASE(Int8);
		CASE(Map);
		CASE(Set);
		CASE(Enum);

#undef CASE
#undef CASE_STR
	}

	// Parsing of type isn't supported
}

void UObject::AddProperty(FPropertyTag& Tag, std::any& Property)
{
	Properties.emplace_back(Tag.Name.Name->Name, Property);
}
