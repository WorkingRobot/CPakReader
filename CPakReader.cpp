#include "src/CIndex.h"
#include "src/HAES.h"

#include <chrono>

namespace ch = std::chrono;

int main(int argc, char* argv[])
{
	char* dirPath;
	FAESKey key;
	if (argc >= 3) {
		dirPath = argv[1];
		key = FAESKey::FromHex(argv[2]);
	}
	else {
		dirPath = "J:\\Programs\\FN\\cache\\fn\\FortniteGame\\Content\\Paks\\";
		key = FAESKey::FromHex("7f851d7e72c27cb0bfd76878e249801373dec3c40707af9a0c93fd7fc5153dbf");
		printf("Command args are \"<path> <key (no 0x)>\", continuing with defaults\n");
	}
	

	CIndex index;
	EErrorCode code;
	auto start = ch::steady_clock::now();
	for (auto& file : fs::directory_iterator(dirPath)) {
		index.AddPak(file.path(), code);
	}
	auto checkpoint = ch::steady_clock::now();
	auto paksLoaded = index.UseKey(key, FGuid::Zero(), code);
	auto end = ch::steady_clock::now();

	printf("Got info in %.2f ms\n", (checkpoint - start).count() / 1000000.f);
	printf("Got index in %.2f ms\n", (end - checkpoint).count() / 1000000.f);
	printf("Total time is %.2f ms\n", (end - start).count() / 1000000.f);
	printf("%d paks were loaded\n", paksLoaded);
	printf("Press any key to continue...");
	getchar();
	return 0;
}
