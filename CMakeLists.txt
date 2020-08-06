cmake_minimum_required (VERSION 3.8)

project (CPakReader)

add_executable (CPakReader
    "CPakReader.cpp"
    "src/CIndex.cpp"
    "src/Readers/RAssetReader.cpp"
    "src/Objects/FName.cpp"
    "src/Objects/FPackageIndex.cpp"
    "src/Exports/UObject.cpp"
    "src/CPackage.cpp"
    "src/Objects/FText.cpp"
    "src/Exports/UTexture2D.cpp"
    "src/Exports/UDataTable.cpp"
    "src/Exports/UCurveTable.cpp"
    "src/Exports/UStringTable.cpp"
    "src/Exports/USoundWave.cpp")

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