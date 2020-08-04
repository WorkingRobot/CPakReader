#pragma once

#include "../Exports/UObject.h"
#include "../Objects/FGameplayTagContainer.h"
#include "../Objects/FQuat.h"
#include "../Objects/FVector2D.h"
#include "../Objects/FVector.h"
#include "../Objects/FRotator.h"
#include "../Objects/FIntPoint.h"
#include "../Objects/FGuid.h"
#include "../Objects/FSoftObjectPath.h"
#include "../Objects/FColor.h"
#include "../Objects/FLinearColor.h"

struct PStruct {
	std::any Value;

	// I'd make this a constructor, but you can't have a template parameter in one
	template<EReadType ReadType>
	static PStruct Create(CStream& InputStream, FPropertyTag& Tag) {
		if (Tag.StructName.IsNull()) {
			LOG_ERR("Struct name is null (PStruct)");
			return {};
		}

		switch (HStringHash::Crc32RT(Tag.StructName.c_str()))
		{
#define CASE(t) case HStringHash::Crc32(#t): { F##t Val; InputStream >> Val; return { Val }; }

			CASE(GameplayTagContainer);
			CASE(Quat);
			CASE(Vector2D);
			CASE(Vector);
			CASE(Rotator);
			CASE(IntPoint);
			CASE(Guid);
			CASE(SoftObjectPath);
			CASE(Color);
			CASE(LinearColor);

#undef CASE
#undef CASE_STR

		default:
			InputStream.SetProperty(CStream::PropStructFallback, true);
			UObject Val;
			InputStream >> Val;
			return { Val };
		}
	}
};
