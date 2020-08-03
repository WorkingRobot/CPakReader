#include "CPackage.h"

#include "CPackageFileStream.h"

void CPackage::Dump(const char* FileName)
{
	for (auto& File : Files) {
		char NameBuf[_MAX_PATH];
		sprintf(NameBuf, "%s.%s", FileName, EAssetTypeHelper::GetString(File.first));

		CPackageFileStream Stream(File.second);
		auto FileBuf = std::unique_ptr<char[]>(new char[Stream.size()]);
		Stream.read(FileBuf.get(), Stream.size());

		auto FilePtr = fopen(NameBuf, "wb");
		fwrite(FileBuf.get(), Stream.size(), 1, FilePtr);
		fclose(FilePtr);
	}
	for (auto& File : OtherFiles) {
		char NameBuf[_MAX_PATH];
		sprintf(NameBuf, "%s.%.*s", FileName, File.first.NameSize, File.first.Name.get());

		CPackageFileStream Stream(File.second);
		auto FileBuf = std::unique_ptr<char[]>(new char[Stream.size()]);
		Stream.read(FileBuf.get(), Stream.size());

		auto FilePtr = fopen(NameBuf, "wb");
		fwrite(FileBuf.get(), Stream.size(), 1, FilePtr);
		fclose(FilePtr);
	}
}
