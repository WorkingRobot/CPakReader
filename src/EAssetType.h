#pragma once

#include "HStringHash.h"

#include <string.h>

enum class EAssetType : uint8_t {
	UPROJECT,			// FortniteGame.uproject, just json
	UPLUGINMANIFEST,	// FortniteGame.upluginmanifest, just json
	UPLUGIN,			// just json

	UASSET,				// name maps, export/import maps, etc
	UEXP,				// asset data
	UMAP,				// maps
	UBULK,				// images, skins, etc
	UDIC,				// oodle dictionary network compression
	
	USHADERBYTECODE,	// shader bytecode
	UPIPELINECACHE,		// something?

	LOCMETA,			// localization
	LOCRES,				// localization

	PNG,				// images
	PSD,				// photoshop psd

	UFONT,				// otf format
	TTF,				// font
	TPS,				// font metadata, xml

	ICU,				// icudt64l stuff
	RES,				// icudt64l stuff
	CFU,				// icudt64l stuff
	NRM,				// icudt64l stuff
	BRK,				// icudt64l stuff
	DICT,				// icudt64l stuff

	BIN,				// shader cache, asset registry, PCM audio (Content/VoiceData/player_message_01_24k.bin)
	INI,				// hotfixes, engine inis, etc
	PEM,				// cacert.pem
	K,					// facial animations, they're zip files with stuff in em

	UNKNOWN = 0xFF,		// extension is not known
};

class EAssetTypeHelper {
public:
	static constexpr EAssetType GetType(const char* extension, int extensionSize) {
		switch (HStringHash::Crc32<true>(extension, extensionSize))
		{
#define CASE(t) case HStringHash::Crc32<true>(#t): return EAssetType::##t

		CASE(UPROJECT);
		CASE(UPLUGINMANIFEST);
		CASE(UPLUGIN);

		CASE(UASSET);
		CASE(UEXP);
		CASE(UMAP);
		CASE(UBULK);
		CASE(UDIC);

		CASE(USHADERBYTECODE);
		CASE(UPIPELINECACHE);

		CASE(LOCMETA);
		CASE(LOCRES);

		CASE(PNG);
		CASE(PSD);

		CASE(UFONT);
		CASE(TTF);
		CASE(TPS);

		CASE(ICU);
		CASE(RES);
		CASE(CFU);
		CASE(NRM);
		CASE(BRK);
		CASE(DICT);

		CASE(BIN);
		CASE(INI);
		CASE(PEM);
		CASE(K);

#undef CASE

		default:
			return EAssetType::UNKNOWN;
		}
	}

	static constexpr const char* GetString(EAssetType type) {
		switch (type)
		{
		case EAssetType::UPROJECT:			return "uproject";
		case EAssetType::UPLUGINMANIFEST:	return "upluginmanifest";
		case EAssetType::UPLUGIN:			return "uplugin";
		case EAssetType::UASSET:			return "uasset";
		case EAssetType::UEXP:				return "uexp";
		case EAssetType::UMAP:				return "umap";
		case EAssetType::UBULK:				return "ubulk";
		case EAssetType::UDIC:				return "udic";
		case EAssetType::USHADERBYTECODE:	return "ushaderbytecode";
		case EAssetType::UPIPELINECACHE:	return "upipelinecache";
		case EAssetType::LOCMETA:			return "locmeta";
		case EAssetType::LOCRES:			return "locres";
		case EAssetType::PNG:				return "png";
		case EAssetType::PSD:				return "psd";
		case EAssetType::UFONT:				return "ufont";
		case EAssetType::TTF:				return "ttf";
		case EAssetType::TPS:				return "tps";
		case EAssetType::ICU:				return "icu";
		case EAssetType::RES:				return "res";
		case EAssetType::CFU:				return "cfu";
		case EAssetType::NRM:				return "nrm";
		case EAssetType::BRK:				return "brk";
		case EAssetType::DICT:				return "dict";
		case EAssetType::BIN:				return "bin";
		case EAssetType::INI:				return "ini";
		case EAssetType::PEM:				return "pem";
		case EAssetType::K:					return "k";
		default:							return "";
		}
	}
};