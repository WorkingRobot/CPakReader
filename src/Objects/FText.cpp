#include "FText.h"

#include "ETextHistoryType.h"
#include "FTextHistory.h"

CStream& operator>>(CStream& InputStream, FText& Text) {
	uint32_t Flags;
	InputStream >> Flags;

	// "Assuming" the reader/archive is persistent
	Flags &= ConvertedProperty | InitializedFromString;

	Text.Flags = (ETextFlag)Flags;

	// Execute if UE4 version is at least VER_UE4_FTEXT_HISTORY

	// The type is serialized during the serialization of the history, during deserialization we need to deserialize it and create the correct history
	int8_t HistoryType;
	InputStream >> HistoryType;

	// https://github.com/EpicGames/UnrealEngine/blob/bf95c2cbc703123e08ab54e3ceccdd47e48d224a/Engine/Source/Runtime/Core/Private/Internationalization/TextHistory.cpp
	// https://github.com/EpicGames/UnrealEngine/blob/bf95c2cbc703123e08ab54e3ceccdd47e48d224a/Engine/Source/Runtime/Core/Private/Internationalization/TextData.h
	switch ((ETextHistoryType)HistoryType)
	{
#define CASE(t) case t: { History_##t Val; InputStream >> Val; Text.Text = std::make_shared<FTextHistory>(std::move(Val)); break; }

		CASE(None);
		CASE(Base);
		CASE(NamedFormat);
		CASE(OrderedFormat);
		CASE(ArgumentFormat);
		CASE(AsNumber);
		CASE(AsPercent);
		CASE(AsCurrency);
		CASE(AsDate);
		CASE(AsTime);
		CASE(AsDateTime);
		CASE(Transform);
		CASE(StringTableEntry);
		CASE(TextGenerator);

#undef CASE
	}

	return InputStream;
}