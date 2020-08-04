cmake_minimum_required (VERSION 3.8)

project (CPakReader)

add_executable (CPakReader "CPakReader.cpp"  "src/CIndex.h" "src/CPackage.h" "src/EAssetType.h" "src/CIndex.cpp" "src/HStringHash.h" "src/CFileTree.h" "src/EErrorCode.h" "src/Objects/FAESKey.h" "src/Objects/EBulkDataFlags.h" "src/Objects/ECompressionFlags.h" "src/Objects/ECurveTableMode.h" "src/Objects/EDateTimeStyle.h" "src/Objects/EObjectFlags.h" "src/Objects/EPackageFlags.h" "src/Objects/EPakVersion.h" "src/Objects/EPixelFormat.h" "src/Objects/ETextFlag.h" "src/Objects/ETextHistoryType.h" "src/Objects/FPakInfo.h" "src/Objects/FSHAHash.h" "src/CStream.h" "src/CFileStream.h" "src/CMemoryStream.h" "src/HAES.h"  "src/CPackageFile.h" "src/Objects/FPakEntry.h" "src/Objects/FPakCompressedBlock.h" "src/FPakFile.h"   "src/CPackageFileStream.h" "src/HAlign.h" "src/ECompressionMethod.h" "libraries/libdeflate/libdeflate.h" "src/Readers/RAssetReader.h" "src/Readers/RAssetReader.cpp" "src/Objects/FPackageFileSummary.h" "src/Objects/FCustomVersionContainer.h" "src/Objects/FCustomVersion.h" "src/Objects/FEngineVersion.h" "src/Objects/FCompressedChunk.h" "src/Objects/FGenerationInfo.h" "src/Objects/FNameEntrySerialized.h"  "src/Objects/FName.h" "src/Objects/FObjectResource.h" "src/Objects/FPackageIndex.h" "src/Objects/FObjectImport.h" "src/Objects/FObjectExport.h" "src/Objects/FName.cpp" "src/Objects/FPackageIndex.cpp" "libraries/xxhash/xxh3.h"    "src/Exports/UObject.h" "src/Exports/UObject.cpp" "src/Objects/FPropertyTag.h" "src/Properties/PByte.h" "src/EReadType.h" "src/Properties/PBool.h" "src/Properties/PInt.h" "src/Properties/PObject.h" "src/Properties/PDelegate.h" "src/Properties/PDouble.h" "src/Properties/PStruct.h" "src/Properties/PStr.h" "src/Properties/PText.h" "src/Properties/PInterface.h" "src/Properties/PMulticastDelegate.h" "src/Properties/PLazyObject.h" "src/Properties/PUInt64.h" "src/Properties/PUInt16.h" "src/Properties/PInt64.h" "src/Properties/PMap.h" "src/Properties/PEnum.h" "src/Properties/PSoftObjectMap.h" "src/Objects/FSoftObjectPath.h" "src/Objects/FText.h" "src/Objects/FDateTime.h" "src/Objects/FGameplayTagContainer.h" "src/Objects/FQuat.h" "src/Objects/FVector.h" "src/Objects/FRotator.h" "src/Objects/FColor.h" "src/Objects/FLinearColor.h" "src/CPackage.cpp" "src/Objects/ETransformType.h" "src/Objects/FFormatArgumentValue.h" "src/Objects/EFormatArgumentType.h" "src/Objects/FFormatNamedArguments.h" "src/Objects/FFormatArgumentData" "src/Objects/ETextGender.h" "src/Objects/FNumberFormattingOptions.h" "src/Objects/ERoundingMode.h" "src/Objects/FTextHistory.h" "src/Objects/FText.cpp" "src/Exports/UTexture2D.h" "src/Exports/UTexture2D.cpp" "src/Objects/FTexturePlatformData.h" "src/Objects/FOptTexturePlatformData.h" "src/Objects/FVirtualTextureBuiltData.h" "src/Objects/FTexture2DMipMap.h" "src/Objects/FByteBulkData.h" "src/Objects/FVirtualTextureDataChunk.h" "src/Objects/EVirtualTextureCodec.h" "src/Objects/FStripDataFlags.h")

set_property(TARGET CPakReader PROPERTY CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")
   target_compile_options(CPakReader PRIVATE /Zi)
    set_target_properties(CPakReader PROPERTIES
        LINK_FLAGS "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF"
        COMPILE_PDB_NAME CPakReader 
        COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
    )
endif()

target_include_directories(CPakReader PRIVATE
    "libraries/libdeflate"
    "libraries/xxhash")

target_link_libraries(CPakReader PRIVATE
    "${CMAKE_SOURCE_DIR}/libraries/libdeflate/libdeflatestatic.lib"
    "${CMAKE_SOURCE_DIR}/libraries/xxhash/xxhash.lib")