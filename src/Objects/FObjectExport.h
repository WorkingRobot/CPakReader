#pragma once

#include "../CStream.h"
#include "EObjectFlags.h"
#include "FObjectResource.h"

struct FObjectExport : public FObjectResource {
	FPackageIndex ClassIndex;
	FPackageIndex SuperIndex;

	// FileVersionUE4 >= VER_UE4_TemplateIndex_IN_COOKED_EXPORTS
	FPackageIndex TemplateIndex;
	EObjectFlags ObjectFlags;

	// FileVersionUE4 >= VER_UE4_64BIT_EXPORTMAP_SERIALSIZES
	int64_t SerialSize;
	int64_t SerialOffset;
	bool bForcedExport;
	bool bNotForClient;
	bool bNotForServer;
	FGuid PackageGuid;
	uint32_t PackageFlags;

	// FileVersionUE4 >= VER_UE4_LOAD_FOR_EDITOR_GAME
	bool bNotAlwaysLoadedForEditorGame;

	// FileVersionUE4 >= VER_UE4_COOKED_ASSETS_IN_EDITOR_SUPPORT
	bool bIsAsset;

	// FileVersionUE4 >= VER_UE4_PRELOAD_DEPENDENCIES_IN_COOKED_EXPORTS
	int FirstExportDependency;
	int SerializationBeforeSerializationDependencies;
	int CreateBeforeSerializationDependencies;
	int SerializationBeforeCreateDependencies;
	int CreateBeforeCreateDependencies;

	friend CStream& operator>>(CStream& InputStream, FObjectExport& ObjectExport) {
		InputStream >> ObjectExport.ClassIndex;
		InputStream >> ObjectExport.SuperIndex;

		// FileVersionUE4 >= VER_UE4_TemplateIndex_IN_COOKED_EXPORTS
		InputStream >> ObjectExport.TemplateIndex;
		InputStream >> ObjectExport.OuterIndex;
		InputStream >> ObjectExport.ObjectName;
		uint32_t ObjectFlags;
		InputStream >> ObjectFlags;
		ObjectExport.ObjectFlags = (EObjectFlags)(ObjectFlags & EObjectFlags::RF_Load);

		// FileVersionUE4 >= VER_UE4_64BIT_EXPORTMAP_SERIALSIZES
		InputStream >> ObjectExport.SerialSize;
		InputStream >> ObjectExport.SerialOffset;
		int bTempFlag;
		InputStream >> bTempFlag;
		ObjectExport.bForcedExport = bTempFlag != 0;
		InputStream >> bTempFlag;
		ObjectExport.bNotForClient = bTempFlag != 0;
		InputStream >> bTempFlag;
		ObjectExport.bNotForServer = bTempFlag != 0;
		InputStream >> ObjectExport.PackageGuid;
		InputStream >> ObjectExport.PackageFlags;

		// FileVersionUE4 >= VER_UE4_LOAD_FOR_EDITOR_GAME
		InputStream >> bTempFlag;
		ObjectExport.bNotAlwaysLoadedForEditorGame = bTempFlag != 0;

		// FileVersionUE4 >= VER_UE4_COOKED_ASSETS_IN_EDITOR_SUPPORT
		InputStream >> bTempFlag;
		ObjectExport.bIsAsset = bTempFlag != 0;

		// FileVersionUE4 >= VER_UE4_PRELOAD_DEPENDENCIES_IN_COOKED_EXPORTS
		InputStream >> ObjectExport.FirstExportDependency;
		InputStream >> ObjectExport.SerializationBeforeSerializationDependencies;
		InputStream >> ObjectExport.CreateBeforeSerializationDependencies;
		InputStream >> ObjectExport.SerializationBeforeCreateDependencies;
		InputStream >> ObjectExport.CreateBeforeCreateDependencies;

		return InputStream;
	}
};