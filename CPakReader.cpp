#include "src/CIndex.h"
#include "src/CPackageFileStream.h"
#include "src/HAES.h"
#include "src/Readers/RAssetReader.h"

#include <chrono>
#include <optional>

namespace ch = std::chrono;

int main(int argc, char* argv[])
{
	FAESKey Key = FAESKey::FromHex("7f851d7e72c27cb0bfd76878e249801373dec3c40707af9a0c93fd7fc5153dbf");
	fs::path DirPath = "J:\\Programs\\FN\\cache\\fn\\FortniteGame\\Content\\Paks\\";
	if (argc >= 3) {
		DirPath = argv[1];
		Key = FAESKey::FromHex(argv[2]);
	}

	CIndex index;
	EErrorCode code;
	for (auto& file : fs::directory_iterator(DirPath)) {
		index.AddPak(file.path(), code);
	}
	auto start = ch::steady_clock::now();
	int paksLoaded = index.UseKey(Key, FGuid::Zero(), code);
	auto end = ch::steady_clock::now();
	printf("Loaded %d paks in %.2f ms\n", paksLoaded, (end - start).count() / 1000000.f);
	RAssetReader reader(index.GetPackage("FortniteGame/Content/Catalog/DisplayAssets/DA_Featured_CID_803_Athena_Commando_F_SharkSuit"));

	/*
	auto project = index.GetPackageFile("FortniteGame/Plugins/FortniteGame.upluginmanifest");
	CPackageFileStream reader(*project);
	auto readBuffer = std::make_unique<char[]>(reader.size());
	reader.read(readBuffer.get(), reader.size());
	printf("%s\n", readBuffer.get());
	*/
	return 0;
}
