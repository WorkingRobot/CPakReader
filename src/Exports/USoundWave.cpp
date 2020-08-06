#include "USoundWave.h"

#include "../Properties/PBool.h"

CStream& operator>>(CStream& InputStream, USoundWave& SoundWave) {
	// https://github.com/EpicGames/UnrealEngine/blob/a5dd69186bcff6cf62ed118c70edb3c695c604bd/Engine/Source/Runtime/Engine/Private/SoundBase.cpp#L234
	// https://github.com/EpicGames/UnrealEngine/blob/c39458e354cc77dbfd9abddac3ba542ccad46b8f/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L334
	InputStream >> SoundWave.Object;

	bool bCooked;
	InputStream >> bCooked;

	// FModel considers this to be true as a default, but UE4 makes it false by default?
	// https://github.com/EpicGames/UnrealEngine/blob/28155fb6832f3294ddfcf6b5b11dc7785dcace43/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L357
	bool bShouldStreamSound = true;
	// https://github.com/EpicGames/UnrealEngine/blob/17d8e12f69603abd1f7c844eac268070c4c5d359/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L381
	// https://github.com/EpicGames/UnrealEngine/blob/28155fb6832f3294ddfcf6b5b11dc7785dcace43/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L2085
	auto bStreamingProperty = SoundWave.Object.GetProperty<PBool>("bStreaming");
	if (bStreamingProperty) {
		bShouldStreamSound = bStreamingProperty->Value;
	}

	if (!bShouldStreamSound) {
		if (bCooked) {
			// https://github.com/EpicGames/UnrealEngine/blob/17d8e12f69603abd1f7c844eac268070c4c5d359/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L445
			InputStream >> SoundWave.CompressedFormatData;
			// SoundWave.AudioFormat = SoundWave.CompressedFormatData.back().FormatName;
		}
		else {
			// only save the raw data for non-cooked packages
			// https://github.com/EpicGames/UnrealEngine/blob/17d8e12f69603abd1f7c844eac268070c4c5d359/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L453
			InputStream >> SoundWave.RawData;
		}
		InputStream >> SoundWave.CompressedDataGuid;
	}
	else {
		InputStream >> SoundWave.CompressedDataGuid;
		// https://github.com/EpicGames/UnrealEngine/blob/17d8e12f69603abd1f7c844eac268070c4c5d359/Engine/Source/Runtime/Engine/Private/SoundWave.cpp#L467
		// https://github.com/EpicGames/UnrealEngine/blob/17d8e12f69603abd1f7c844eac268070c4c5d359/Engine/Source/Runtime/Engine/Private/AudioDerivedData.cpp#L1496
		InputStream >> SoundWave.RunningPlatformData;
	}

	return InputStream;
}
