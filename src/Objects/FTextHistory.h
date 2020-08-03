#pragma once

#include "../CStream.h"
#include "FText.h"
#include "FFormatNamedArguments.h"
#include "FNumberFormattingOptions.h"
#include "FDateTime.h"
#include "EDateTimeStyle.h"
#include "ETransformType.h"
#include "FName.h"

#include <optional>
#include <variant>

struct History_None {
	std::string CultureInvariantString;

	friend CStream& operator>>(CStream& InputStream, History_None& Text) {
		bool HasCultureInvariantString;
		InputStream >> HasCultureInvariantString;
		if (HasCultureInvariantString) {
			InputStream >> Text.CultureInvariantString;
		}

		return InputStream;
	}
};

struct History_Base {
	std::string Namespace;
	std::string Key;
	std::string SourceString;

	friend CStream& operator>>(CStream& InputStream, History_Base& Text) {
		InputStream >> Text.Namespace;
		InputStream >> Text.Key;
		InputStream >> Text.SourceString;

		return InputStream;
	}
};

struct History_NamedFormat {
	FText SourceFmt;
	FFormatNamedArguments Arguments;

	friend CStream& operator>>(CStream& InputStream, History_NamedFormat& Text) {
		InputStream >> Text.SourceFmt;
		InputStream >> Text.Arguments;

		return InputStream;
	}
};

struct History_OrderedFormat {
	FText SourceFmt;
	FFormatNamedArguments Arguments;

	friend CStream& operator>>(CStream& InputStream, History_OrderedFormat& Text) {
		InputStream >> Text.SourceFmt;
		InputStream >> Text.Arguments;

		return InputStream;
	}
};

struct History_ArgumentFormat {
	FText SourceFmt;
	std::vector<FFormatArgumentValue> Arguments;

	friend CStream& operator>>(CStream& InputStream, History_ArgumentFormat& Text) {
		InputStream >> Text.SourceFmt;

		InputStream >> Text.Arguments;

		return InputStream;
	}
};

struct History_AsNumber {
	FFormatArgumentValue SourceValue;
	// FModel skips FormatOptions and provides a std::string TimeZone (?)
	std::optional<FNumberFormattingOptions> FormatOptions;
	std::string TargetCulture;

	friend CStream& operator>>(CStream& InputStream, History_AsNumber& Text) {
		InputStream >> Text.SourceValue;

		bool bHasFormatOptions;
		InputStream >> bHasFormatOptions;
		if (bHasFormatOptions) {
			FNumberFormattingOptions FormatOptions;
			InputStream >> FormatOptions;
			Text.FormatOptions.emplace(std::move(FormatOptions));
		}

		InputStream >> Text.TargetCulture;

		return InputStream;
	}
};

struct History_AsPercent {
	FFormatArgumentValue SourceValue;
	// FModel skips FormatOptions and provides a std::string TimeZone (?)
	std::optional<FNumberFormattingOptions> FormatOptions;
	std::string TargetCulture;

	friend CStream& operator>>(CStream& InputStream, History_AsPercent& Text) {
		InputStream >> Text.SourceValue;

		bool bHasFormatOptions;
		InputStream >> bHasFormatOptions;
		if (bHasFormatOptions) {
			FNumberFormattingOptions FormatOptions;
			InputStream >> FormatOptions;
			Text.FormatOptions.emplace(std::move(FormatOptions));
		}

		InputStream >> Text.TargetCulture;

		return InputStream;
	}
};

struct History_AsCurrency {
	std::string CurrencyCode;
	FFormatArgumentValue SourceValue;
	// FModel skips FormatOptions and provides a std::string TimeZone (?)
	std::optional<FNumberFormattingOptions> FormatOptions;
	std::string TargetCulture;

	friend CStream& operator>>(CStream& InputStream, History_AsCurrency& Text) {
		InputStream >> Text.CurrencyCode;
		InputStream >> Text.SourceValue;

		bool bHasFormatOptions;
		InputStream >> bHasFormatOptions;
		if (bHasFormatOptions) {
			FNumberFormattingOptions FormatOptions;
			InputStream >> FormatOptions;
			Text.FormatOptions.emplace(std::move(FormatOptions));
		}

		InputStream >> Text.TargetCulture;

		return InputStream;
	}
};

struct History_AsDate {
	FDateTime SourceDateTime;
	EDateTimeStyle DateStyle;
	std::string TimeZone;
	std::string TargetCulture;

	friend CStream& operator>>(CStream& InputStream, History_AsDate& Text) {
		InputStream >> Text.SourceDateTime;

		int8_t DateStyleInt8;
		InputStream >> DateStyleInt8;
		Text.DateStyle = (EDateTimeStyle)DateStyleInt8;

		// UE4Ver >= VER_UE4_FTEXT_HISTORY_DATE_TIMEZONE
		InputStream >> Text.TimeZone;

		InputStream >> Text.TargetCulture;

		return InputStream;
	}
};

struct History_AsTime {
	FDateTime SourceDateTime;
	EDateTimeStyle TimeStyle;
	std::string TimeZone;
	std::string TargetCulture;

	friend CStream& operator>>(CStream& InputStream, History_AsTime& Text) {
		InputStream >> Text.SourceDateTime;

		int8_t TimeStyleInt8;
		InputStream >> TimeStyleInt8;
		Text.TimeStyle = (EDateTimeStyle)TimeStyleInt8;

		InputStream >> Text.TimeZone;

		InputStream >> Text.TargetCulture;

		return InputStream;
	}
};

struct History_AsDateTime {
	FDateTime SourceDateTime;
	EDateTimeStyle DateStyle;
	EDateTimeStyle TimeStyle;

	std::string TimeZone;

	// UE4 converts the string into an FCulturePtr
	// https://github.com/EpicGames/UnrealEngine/blob/bf95c2cbc703123e08ab54e3ceccdd47e48d224a/Engine/Source/Runtime/Core/Private/Internationalization/TextHistory.cpp#L2188
	std::string TargetCulture;

	friend CStream& operator>>(CStream& InputStream, History_AsDateTime& Text) {
		InputStream >> Text.SourceDateTime;

		int8_t DateStyleInt8;
		InputStream >> DateStyleInt8;
		Text.DateStyle = (EDateTimeStyle)DateStyleInt8;

		int8_t TimeStyleInt8;
		InputStream >> TimeStyleInt8;
		Text.TimeStyle = (EDateTimeStyle)TimeStyleInt8;

		InputStream >> Text.TimeZone;

		InputStream >> Text.TargetCulture;

		return InputStream;
	}
};

struct History_Transform {
	FText SourceText;
	ETransformType TransformType;

	friend CStream& operator>>(CStream& InputStream, History_Transform& Text) {
		InputStream >> Text.SourceText;
		uint8_t TransformType;
		InputStream >> TransformType;
		Text.TransformType = (ETransformType)TransformType;

		return InputStream;
	}
};

struct History_StringTableEntry {
	FName TableId;
	std::string Key;

	friend CStream& operator>>(CStream& InputStream, History_StringTableEntry& Text) {
		InputStream >> Text.TableId;
		InputStream >> Text.Key;

		return InputStream;
	}
};

struct History_TextGenerator {
	FName GeneratorTypeID;
	std::vector<uint8_t> GeneratorContents;

	friend CStream& operator>>(CStream& InputStream, History_TextGenerator& Text) {
		InputStream >> Text.GeneratorTypeID;

		if (!Text.GeneratorTypeID.IsNone()) {
			InputStream >> Text.GeneratorContents;
		}

		return InputStream;
	}
};

// awkward workaround, but it works. I'm not sure of a much better way of doing this
struct FTextHistory {
	std::variant<
		History_None,
		History_Base,
		History_NamedFormat,
		History_OrderedFormat,
		History_ArgumentFormat,
		History_AsNumber,
		History_AsPercent,
		History_AsCurrency,
		History_AsDate,
		History_AsTime,
		History_AsDateTime,
		History_Transform,
		History_StringTableEntry,
		History_TextGenerator
	> Value;

#define CASE(n) FTextHistory(const History_##n&& e) : Value(e) {}

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
};