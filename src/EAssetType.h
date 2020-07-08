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
	static EAssetType GetType(const char* extension) {
		switch (HStringHash::Crc32<true>(extension, strlen(extension)))
		{
#define CASE(t) case HStringHash::Crc32CE(#t): return EAssetType::##t

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
};