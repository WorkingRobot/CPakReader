#pragma once

#include "../CPackage.h"
#include "../CPackageFileStream.h"
#include "../CStream.h"
#include "../Objects/FName.h"
#include "../Objects/FNameEntrySerialized.h"
#include "../Objects/FObjectExport.h"
#include "../Objects/FObjectImport.h"
#include "../Objects/FPackageFileSummary.h"
#include "../Objects/FPackageIndex.h"

#include <any>

class RAssetReader : public CStream {
public:
	RAssetReader(const CPackage& Package);

	CStream& read(char* Buf, size_t BufCount) override;
	CStream& seek(size_t Position, SeekPosition SeekFrom) override;
	size_t tell() override;
	size_t size() override;
	void* GetParentPackage() override;

    void PopulateName(FName& Name) {
		if (Name.Index >= 0 && Name.Index < NameMap.size()) {
			Name.Name = &NameMap[Name.Index];
		}
    }

	FObjectResource* GetResource(FPackageIndex& Index) {
        if (Index.IsImport()) {
            return (FObjectResource*)&ImportMap[Index.ToImport()];
        }
        else if (Index.IsExport()) {
            return (FObjectResource*)&ExportMap[Index.ToExport()];
        }
		else {
			return nullptr;
		}
    }

	const CPackage& Package;
	FPackageFileSummary Summary;
	std::vector<FNameEntrySerialized> NameMap;
	std::vector<FObjectImport> ImportMap;
	std::vector<FObjectExport> ExportMap;

	// Not sure what to make this, but std::any seems like the best way to go about it
	std::vector<std::any> Exports;

private:
	void ReadUAsset();
	void ReadUExp();

	CPackageFileStream* BaseStream;
};