#include "CIndex.h"

#include "CFileStream.h"

CIndex::~CIndex()
{
	for (auto& File : PakFiles) {
		delete File.Stream;
	}
}

bool CIndex::AddPak(const fs::path& FilePath, EErrorCode& ErrorCode)
{
	auto& File = PakFiles.emplace_back();
	
	File.Stream = new CFileStream(FilePath);
	File.Stream->seek(-221, CStream::End);
	*File.Stream >> File.Info;
	if (!File.Info.Magic) {
		ErrorCode = EErrorCode::INVALID_PAKINFO;
		return false;
	}

	return true;
}

bool CIndex::AddPak(CStream* InputStream, EErrorCode& ErrorCode)
{
	auto& File = PakFiles.emplace_back();

	File.Stream = InputStream;
	File.Stream->seek(-221, CStream::End);
	*File.Stream >> File.Info;
	if (!File.Info.Magic) {
		ErrorCode = EErrorCode::INVALID_PAKINFO;
		return false;
	}

	return true;
}

bool CIndex::UseKey(const FAESKey& Key, const FGuid& Guid, EErrorCode& ErrorCode)
{
	return true;
}
