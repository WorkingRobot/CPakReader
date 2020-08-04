#pragma once

#include "../CStream.h"

#include "FVirtualTextureDataChunk.h"

struct FVirtualTextureBuiltData {
	uint32_t NumLayers;
	uint32_t NumMips;
	// Width of the texture in pixels. Note the physical width may be larger due to tiling
	uint32_t Width;
	// Height of the texture in pixels. Note the physical height may be larger due to tiling
	uint32_t Height;
	// Number of UDIM blocks that make up the texture, used to compute UV scaling factor
	uint32_t WidthInBlocks;
	uint32_t HeightInBlocks;
	// Tile size excluding borders
	uint32_t TileSize;
	// A BorderSize pixel border will be added around all tiles
	uint32_t TileBorderSize;

	// The pixel format output of the data on the i'th layer. The actual data
	// may still be compressed but will decompress to this pixel format (e.g. zipped DXT5 data).
	EPixelFormat LayerTypes[8];

	// Tile data is packed into separate chunks, typically there is 1 mip level in each chunk for high resolution mips.
	// After a certain threshold, all remaining low resolution mips will be packed into one final chunk.
	std::vector<FVirtualTextureDataChunk> Chunks;

	// Index of the first tile within each chunk
	std::vector<uint32_t> TileIndexPerChunk;

	// Index of the first tile within each mip level
	std::vector<uint32_t> TileIndexPerMip;

	// Info for the tiles organized per level. Within a level tile info is organized in Morton order.
	// This is in morton order which can waste a lot of space in this array for non-square images
	// e.g.:
	// - An 8x1 tile image will allocate 8x4 indexes in this array.
	// - An 1x8 tile image will allocate 8x8 indexes in this array.
	std::vector<uint32_t> TileOffsetInChunk;

    friend CStream& operator>>(CStream& InputStream, FVirtualTextureBuiltData& VirtualTextureBuiltData) {
		// https://github.com/EpicGames/UnrealEngine/blob/f8f4b403eb682ffc055613c7caf9d2ba5df7f319/Engine/Source/Runtime/Engine/Private/VT/VirtualTextureBuiltData.h#L67
		// https://github.com/EpicGames/UnrealEngine/blob/f8f4b403eb682ffc055613c7caf9d2ba5df7f319/Engine/Source/Runtime/Engine/Private/VT/VirtualTextureBuiltData.cpp#L44

		bool bCooked;
		InputStream >> bCooked;

		InputStream >> VirtualTextureBuiltData.NumLayers;
		InputStream >> VirtualTextureBuiltData.WidthInBlocks;
		InputStream >> VirtualTextureBuiltData.HeightInBlocks;
		InputStream >> VirtualTextureBuiltData.TileSize;
		InputStream >> VirtualTextureBuiltData.TileBorderSize;

		// FirstMipToSerialize is always 0 when loading
		// bStripMips = FirstMipToSerialize > 0
		// !bStripMips = always true
		InputStream >> VirtualTextureBuiltData.NumMips;
		InputStream >> VirtualTextureBuiltData.Width;
		InputStream >> VirtualTextureBuiltData.Height;
		InputStream >> VirtualTextureBuiltData.TileIndexPerChunk;
		InputStream >> VirtualTextureBuiltData.TileIndexPerMip;
		InputStream >> VirtualTextureBuiltData.TileOffsetInChunk;

		for (int Layer = 0; Layer < VirtualTextureBuiltData.NumLayers; Layer++)
		{
			std::string PixelFormatString;
			InputStream >> PixelFormatString;
			VirtualTextureBuiltData.LayerTypes[Layer] = EPixelFormatHelper::GetType(PixelFormatString.c_str());
		}

		// Serialize the chunks
		int NumChunksToSerialize;
		InputStream >> NumChunksToSerialize;

		VirtualTextureBuiltData.Chunks.reserve(NumChunksToSerialize);

		for (int ChunkId = 0; ChunkId < VirtualTextureBuiltData.Chunks.size(); ChunkId++)
		{
			auto& Chunk = VirtualTextureBuiltData.Chunks.emplace_back();

			InputStream >> Chunk.SizeInBytes;
			InputStream >> Chunk.CodecPayloadSize;
			for (int LayerIndex = 0; LayerIndex < VirtualTextureBuiltData.NumLayers; ++LayerIndex)
			{
				uint8_t CodecTypeAsByte;
				InputStream >> CodecTypeAsByte;
				Chunk.CodecType[LayerIndex] = (EVirtualTextureCodec)CodecTypeAsByte;
				InputStream >> Chunk.CodecPayloadOffset[LayerIndex];
			}

			InputStream >> Chunk.BulkData;
		}

        return InputStream;
    }
};