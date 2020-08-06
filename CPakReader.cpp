#include "src/CIndex.h"
#include "src/CPackageFileStream.h"
#include "src/HAES.h"
#include "src/Readers/RAssetReader.h"

#include <chrono>
#include <optional>
#include "src/Exports/UObject.h"
#include "src/Properties/PArray.h"
#include "src/Properties/PObject.h"
#include "src/Exports/USoundWave.h"

namespace ch = std::chrono;

int main(int argc, char* argv[])
{
	FAESKey Key = FAESKey::FromHex("450ec70da26dfeea5ec2415a0745316db399305e8a32fbe09e57b1fcc4bd771d");
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
	auto& pkg = index.GetPackage("FortniteGame/Content/Athena/Items/Cosmetics/Characters/CID_397_Athena_Commando_F_TreasureHunterFashion");
	pkg.Dump("versefn");
	RAssetReader reader(pkg);
	for (auto& n : reader.Exports) {
		printf("%s\n", n.type().name());
		auto obj = std::any_cast<UObject>(&n);
		for (auto& p : obj->Properties) {
			printf(" %s: %s\n", p.first.c_str(), p.second.type().name());
		}
	}

	/*
	auto project = index.GetPackageFile("FortniteGame/Plugins/FortniteGame.upluginmanifest");
	CPackageFileStream reader(*project);
	auto readBuffer = std::make_unique<char[]>(reader.size());
	reader.read(readBuffer.get(), reader.size());
	printf("%s\n", readBuffer.get());
	*/
	return 0;
}
