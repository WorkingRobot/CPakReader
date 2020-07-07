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

class RAssetReader : public CStream {
public:
	RAssetReader(const CPackage& Package);

	CStream& read(char* Buf, size_t BufCount) override;
	CStream& seek(size_t Position, SeekPosition SeekFrom) override;
	size_t size() override;

    void PopulateName(FName& Name) {
        Name.Name = &NameMap[Name.Index];
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

	FPackageFileSummary Summary;
	std::vector<FNameEntrySerialized> NameMap;
	std::vector<FObjectImport> ImportMap;
	std::vector<FObjectExport> ExportMap;

private:
	void ReadUAsset();
	void ReadUExp();

	CPackageFileStream* BaseStream;
};