#include "CIndex.h"

#include "CFileStream.h"
#include "CMemoryStream.h"
#include "FPakFile.h"
#include "HAES.h"
#include "HAlign.h"

CIndex::~CIndex()
{
	for (auto& File : PakFiles) {
		delete File.Stream;
	}
}

bool CIndex::AddPak(const fs::path& FilePath, EErrorCode& ErrorCode)
{
	FPakFile File;

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
	FPakFile File;

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

				int NumEntries;
				BufferStream >> NumEntries;
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

				File.Stream->seek(DirectoryOffset, CStream::Begin);
				std::shared_ptr<char[]> DirectoryData = std::shared_ptr<char[]>(new char[DirectorySize]);
				File.Stream->read(DirectoryData.get(), DirectorySize);

				if (File.Info.bEncryptedIndex)
				{
					HAES::DecodeInPlace(Key, DirectoryData.get(), DirectorySize);
				}

				CMemoryStream DirectoryStream(DirectoryData, DirectorySize);
				std::vector<FPakDirectoryEntry> DirectoryEntries;
				DirectoryStream >> DirectoryEntries;

				for (auto& DirectoryEntry : DirectoryEntries) {
					for (auto& HashIndexEntry : DirectoryEntry.Entries) {
						FPakEntry Entry;
						GetEncodedPakEntry(EncodedPakEntries.get() + HashIndexEntry.Location, (EPakVersion)File.Info.Version, Entry);
						if (MountPoint.empty()) {
							FileTree.AddEntry(DirectoryEntry.Directory.c_str(), HashIndexEntry.Filename.c_str(), File, Entry);
						}
						else {
							// maybe make this more performant? not sure how std::string handles this
							FileTree.AddEntry((MountPoint + DirectoryEntry.Directory).c_str(), HashIndexEntry.Filename.c_str(), File, Entry);
						}
					}
				}
			}
			else {
				printf("Old pak versions not implemented yet!\n");
			}

			File.Key = Key;
			File.Initialized = true;
			++LoadedFiles;
		}
	}
	return LoadedFiles;
}

CPackage* CIndex::GetPackage(const char* Path)
{
	return FileTree.GetPackageEntry(Path);
}

CPackageFile* CIndex::GetPackageFile(const char* Path)
{
	return FileTree.GetFileEntry(Path);
}

void CIndex::GetEncodedPakEntry(const char* EncodedPtr, EPakVersion Version, FPakEntry& Entry)
{
	// Grab the big bitfield value:
	// Bit 31 = Offset 32-bit safe?
	// Bit 30 = Uncompressed size 32-bit safe?
	// Bit 29 = Size 32-bit safe?
	// Bits 28-23 = Compression method
	// Bit 22 = Encrypted
	// Bits 21-6 = Compression blocks count
	// Bits 5-0 = Compression block size
	uint32_t Value = *(uint32_t*)EncodedPtr;
	EncodedPtr += sizeof(Value);

	Entry.CompressionMethodIndex = (Value >> 23) & 0x3f;

	if ((Value & (1 << 31)) != 0) // Offset is 32-bit safe
	{
		Entry.Offset = *(uint32_t*)EncodedPtr;
		EncodedPtr += sizeof(uint32_t);
	}
	else
	{
		memcpy(&Entry.Offset, EncodedPtr, sizeof(int64_t));
		EncodedPtr += sizeof(int64_t);
	}


	if ((Value & (1 << 30)) != 0) // Uncompressed size is 32-bit safe
	{
		Entry.UncompressedSize = *(uint32_t*)EncodedPtr;
		EncodedPtr += sizeof(uint32_t);
	}
	else
	{
		memcpy(&Entry.UncompressedSize, EncodedPtr, sizeof(int64_t));
		EncodedPtr += sizeof(int64_t);
	}

	if (Entry.CompressionMethodIndex != 0)
	{
		if ((Value & (1 << 29)) != 0) // Size is 32-bit safe
		{
			Entry.Size = *(uint32_t*)EncodedPtr;
			EncodedPtr += sizeof(uint32_t);
		}
		else
		{
			memcpy(&Entry.Size, EncodedPtr, sizeof(int64_t));
			EncodedPtr += sizeof(int64_t);
		}
	}
	else
	{
		// No compression
		Entry.Size = Entry.UncompressedSize;
	}

	Entry.Flags = 0;
	if ((Value & (1 << 22)) != 0) {
		Entry.Flags |= FPakEntry::Flag_Encrypted;
	}

	uint32_t CompressionBlockCount = (Value >> 6) & 0xffff;
	Entry.CompressionBlocks.reserve(CompressionBlockCount);

	Entry.CompressionBlockSize = 0;
	if (CompressionBlockCount) {
		Entry.CompressionBlockSize = Entry.UncompressedSize < 65536 ? Entry.UncompressedSize : ((Value & 0x3f) << 11);
	}

	int64_t BaseOffset = Version >= PAKVER_RELATIVE_CHUNK_OFFSETS ? 0 : Entry.Offset;
	if (CompressionBlockCount == 1 && !(Entry.Flags & FPakEntry::Flag_Encrypted))
	{
		FPakCompressedBlock& CompressedBlock = Entry.CompressionBlocks.emplace_back();
		CompressedBlock.CompressedStart = BaseOffset + FPakEntry::GetSerializedSize(Version, Entry.CompressionMethodIndex, CompressionBlockCount);
		CompressedBlock.CompressedEnd = CompressedBlock.CompressedStart + Entry.Size;
	}
	else if (CompressionBlockCount > 0)
	{
		uint32_t* CompressionBlockSizePtr = (uint32_t*)EncodedPtr;
		uint64_t CompressedBlockAlignment = (Entry.Flags & FPakEntry::Flag_Encrypted) ? 16 : 1; // 16 = FAES::AESBlockSize

		int64_t CompressedBlockOffset = BaseOffset + FPakEntry::GetSerializedSize(Version, Entry.CompressionMethodIndex, CompressionBlockCount);
		for (int i = 0; i < CompressionBlockCount; ++i)
		{
			FPakCompressedBlock& CompressedBlock = Entry.CompressionBlocks.emplace_back();
			CompressedBlock.CompressedStart = CompressedBlockOffset;
			CompressedBlock.CompressedEnd = CompressedBlockOffset + *CompressionBlockSizePtr++;
			CompressedBlockOffset += HAlign::Align(CompressedBlock.CompressedEnd - CompressedBlock.CompressedStart, CompressedBlockAlignment);
		}
	}
}
