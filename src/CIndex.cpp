#include "CIndex.h"

#include "CFileStream.h"
#include "HAES.h"
#include "CMemoryStream.h"

CIndex::~CIndex()
{
	for (auto& File : PakFiles) {
		delete File.Stream;
	}
}

bool CIndex::AddPak(const fs::path& FilePath, EErrorCode& ErrorCode)
{
	PakFile File;

	File.Stream = new CFileStream(FilePath);
	File.Stream->seek(-221, CStream::End);
	*File.Stream >> File.Info;
	if (!File.Info.Magic) {
		ErrorCode = EErrorCode::INVALID_PAKINFO;
		return false;
	}
	File.Initialized = false;

	PakFiles.emplace_back(File);
	return true;
}

bool CIndex::AddPak(CStream* InputStream, EErrorCode& ErrorCode)
{
	PakFile File;

	File.Stream = InputStream;
	File.Stream->seek(-221, CStream::End);
	*File.Stream >> File.Info;
	if (!File.Info.Magic) {
		ErrorCode = EErrorCode::INVALID_PAKINFO;
		return false;
	}
	File.Initialized = false;

	PakFiles.emplace_back(File);
	return true;
}

struct FPathHashIndexEntry {
	std::string Filename;

	int Location;

	friend CStream& operator>>(CStream& InputStream, FPathHashIndexEntry& PathHashIndexEntry) {
		InputStream >> PathHashIndexEntry.Filename;
		InputStream >> PathHashIndexEntry.Location;
		return InputStream;
	}
};

struct FPakDirectoryEntry {
	std::string Directory;

	std::vector<FPathHashIndexEntry> Entries;

	friend CStream& operator>>(CStream& InputStream, FPakDirectoryEntry& PakDirectoryEntry) {
		InputStream >> PakDirectoryEntry.Directory;
		InputStream >> PakDirectoryEntry.Entries;
		return InputStream;
	}
};

int CIndex::UseKey(const FAESKey& Key, const FGuid& Guid, EErrorCode& ErrorCode)
{
	int LoadedFiles = 0;
	for (auto& File : PakFiles) {
		if (File.Initialized) {
			continue;
		}

		if (File.Info.EncryptionKeyGuid == Guid) {
			File.Stream->seek(File.Info.IndexOffset, CStream::Begin);
			std::shared_ptr<char[]> IndexBuffer = std::shared_ptr<char[]>(new char[File.Info.IndexSize]);
			File.Stream->read(IndexBuffer.get(), File.Info.IndexSize);

			if (File.Info.bEncryptedIndex) {
				HAES::DecodeInPlace(Key, IndexBuffer.get(), File.Info.IndexSize);
			}
			{
				auto fp = fopen("thing.buf", "wb");
				fwrite(IndexBuffer.get(), File.Info.IndexSize, 1, fp);
				fclose(fp);
			}

			CMemoryStream BufferStream(IndexBuffer, File.Info.IndexSize);
			int StringLength;
			BufferStream >> StringLength;
			if (StringLength > 512 || StringLength < -512) {
				continue;
			}

			if (StringLength < 0) {
				BufferStream.seek((StringLength - 1) * 2, CStream::Cur);

				uint16_t NullTerminator;
				BufferStream >> NullTerminator;
				if (NullTerminator) {
					continue;
				}
			}
			else {
				BufferStream.seek(StringLength - 1, CStream::Cur);

				uint8_t NullTerminator;
				BufferStream >> NullTerminator;
				if (NullTerminator) {
					continue;
				}
			}

			BufferStream.seek(0, CStream::Begin);

			if (File.Info.Version >= PAKVER_PATH_HASH_INDEX) {
				std::string MountPoint;
				BufferStream >> MountPoint;
				if (MountPoint.starts_with("../../..")) {
					MountPoint = MountPoint.substr(9);
				}

				int FileCount;
				BufferStream >> FileCount;
				BufferStream.seek(8, CStream::Cur); // PathHashSeed

				int bReaderHasPathHashIndex;
				BufferStream >> bReaderHasPathHashIndex;
				if (!bReaderHasPathHashIndex) {
					// no path hash index :(
				}

				BufferStream.seek(8 + 8 + 20, CStream::Cur); // PathHashIndexOffset, PathHashIndexSize, PathHashIndexHash

				int bReaderHasFullDirectoryIndex;
				BufferStream >> bReaderHasFullDirectoryIndex;
				if (!bReaderHasFullDirectoryIndex) {
					// no directory index :(
				}

				int64_t DirectoryOffset, DirectorySize;
				BufferStream >> DirectoryOffset;
				BufferStream >> DirectorySize;
				BufferStream.seek(20, CStream::Cur); // FullDirectoryIndexHash

				int EncodedPakEntriesSize;
				BufferStream >> EncodedPakEntriesSize;
				std::unique_ptr<char[]> EncodedPakEntries = std::make_unique<char[]>(EncodedPakEntriesSize);
				BufferStream.read(EncodedPakEntries.get(), EncodedPakEntriesSize);

				int FileCount;
				BufferStream >> FileCount;

				if (FileCount < 0)
				{
					// Corrupt pak PrimaryIndex
				}

				BufferStream.seek(DirectoryOffset, CStream::Begin);
				std::shared_ptr<char[]> DirectoryData = std::shared_ptr<char[]>(new char[DirectorySize]);
				BufferStream.read(DirectoryData.get(), DirectorySize);

				if (File.Info.bEncryptedIndex)
				{
					HAES::DecodeInPlace(Key, DirectoryData.get(), DirectorySize);
				}

				CMemoryStream DirectoryStream(DirectoryData, DirectorySize);
				std::vector<FPakDirectoryEntry> DirectoryEntries;
				DirectoryStream >> DirectoryEntries;

				// TODO: implement DirectoryEntries and add to index
			}
			else {

			}
		}
	}
	return LoadedFiles;
}
