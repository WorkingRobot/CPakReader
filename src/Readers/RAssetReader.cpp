#include "RAssetReader.h"

#include "../CStream.h"
#include "../Objects/FName.h"
#include "../Exports/UObject.h"
#include "../Exports/UTexture2D.h"
#include "../Exports/UDataTable.h"
#include "../Exports/UCurveTable.h"
#include "../Exports/UStringTable.h"

#include <numeric>

RAssetReader::RAssetReader(const CPackage& Package) : Package(Package)
{
	{
		CPackageFileStream UAssetStream(Package, "uasset");
		BaseStream = &UAssetStream;
		ReadUAsset();
	}

	{
		CPackageFileStream UExpStream(Package, "uexp");
		BaseStream = &UExpStream;
		ReadUExp();
	}
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

size_t RAssetReader::tell()
{
	return BaseStream->tell();
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
	int64_t BulkDataOffset = std::accumulate(ExportMap.begin(), ExportMap.end(), 0ll, [](int64_t a, const FObjectExport& b) {return a + b.SerialSize; }) + Summary.TotalHeaderSize;

	this->SetProperty(CStream::PropDataOffset, BulkDataOffset);
	this->SetProperty(CStream::PropParentPackage, (size_t)&Package);

	for (auto& Export : ExportMap) {
		if (true) {//Export.bIsAsset) {
			FName ObjectClassName;

			if (Export.ClassIndex.IsNull()) {
				*this >> ObjectClassName;
			}
			else if (Export.ClassIndex.IsExport()) {
				ObjectClassName = ExportMap[Export.ClassIndex.ToExport()].ObjectName;
			}
			else if (Export.ClassIndex.IsImport()) {
				ObjectClassName = ImportMap[Export.ClassIndex.ToImport()].ObjectName;
			}
			else {
				LOG_ERR("Can't get class name");
				continue;
			}

			auto StartingPosition = Export.SerialOffset - Summary.TotalHeaderSize;
			seek(StartingPosition, CStream::Begin);

			auto& ExpObject = Exports.emplace_back();
			printf("%s\n", ObjectClassName.c_str());
			switch (HStringHash::Crc32RT(ObjectClassName.c_str()))
			{
#define CASE(t, p) case HStringHash::Crc32(#t): { U##t ExpObj; *this >> ExpObj; ExpObject.emplace<U##t>(std::move(ExpObj)); break; }

				CASE(Texture2D);
				CASE(DataTable);
				CASE(CurveTable);
				CASE(StringTable);
				//CASE(FontFace); Not too sure here, aren't fonts just .ufont files?
				//CASE(SoundWave);

#undef CASE

				default:
				{
					UObject ExpObj;
					*this >> ExpObj;
					ExpObject.emplace<UObject>(std::move(ExpObj));
					break;
				}
			}

			if (StartingPosition + Export.SerialSize != tell()) {
				LOG_ERR("Didn't read %s (%s) correctly (at %d, should be %d, %d behind)", Export.ObjectName.c_str(), ObjectClassName.c_str(), tell(), StartingPosition + Export.SerialSize, StartingPosition + Export.SerialSize - tell());
				seek(StartingPosition + Export.SerialSize, CStream::Begin);
			}
		}
	}
}
