#include "RAssetReader.h"

#include "../CStream.h"
#include "../Objects/FName.h"

RAssetReader::RAssetReader(const CPackage& Package)
{
	CPackageFileStream UAssetStream(Package, "uasset");
	BaseStream = &UAssetStream;
	ReadUAsset();
}

CStream& RAssetReader::read(char* Buf, size_t BufCount)
{
	BaseStream->read(Buf, BufCount);
	return *this;
}

CStream& RAssetReader::seek(size_t Position, SeekPosition SeekFrom)
{
	BaseStream->seek(Position, SeekFrom);
	return *this;
}

size_t RAssetReader::size()
{
	return BaseStream->size();
}

void RAssetReader::ReadUAsset()
{
	*this >> Summary;
	
	if (Summary.NameCount > 0) {
		seek(Summary.NameOffset, CStream::Begin);
		NameMap.reserve(Summary.NameCount);
		for (int i = 0; i < Summary.NameCount; ++i) {
			auto& Value = NameMap.emplace_back();
			*this >> Value;
		}
	}

	if (Summary.ImportCount > 0) {
		seek(Summary.ImportOffset, CStream::Begin);
		ImportMap.reserve(Summary.ImportCount);
		for (int i = 0; i < Summary.ImportCount; ++i) {
			auto& Value = ImportMap.emplace_back();
			*this >> Value;
		}
	}

	if (Summary.ExportCount > 0) {
		seek(Summary.ExportOffset, CStream::Begin);
		ExportMap.reserve(Summary.ExportCount);
		for (int i = 0; i < Summary.ExportCount; ++i) {
			auto& Value = ExportMap.emplace_back();
			*this >> Value;
		}
	}
}

void RAssetReader::ReadUExp()
{
	// soon (tm)
}
