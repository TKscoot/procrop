#pragma once
#include <unordered_map>

#include "TileMap.h"
#include "Random.h"
#include "Types.h"
#include "RangedValue.h"
#include "FastNoiseLite/FastNoiseLite.h"

class GridChunk
{
  public:
	GridChunk(TileSheet* tileSheet, Point chunkPosition, Point chunkSize, Point tileScaling);

	void GenerateChunk();
	void SetBackgroundSprite(int x, int y);
	void DrawChunk(Point playerOffset);
	void SetChunkPosition(Point pos){mChunkPosition = pos;}
	Point GetChunkPosition(){return mChunkPosition;}
	void SetTileImage(Point pos, Image* img, ImageLayer layer = ImageLayer::BACKGROUND);

  private:
	Point mChunkIndex = {0,0};
	Point mPosition = {0, 0};
    Point mSize = {20, 20};
    Point mTileScaling = {4, 4};
    std::unordered_map<ImageLayer, std::vector<Image *>> mGrid = {};
	TileSheet* mTileSet = nullptr;
	TileSheet* mVegetationTileSet = nullptr;
	Point mChunkPosition;

	static FastNoiseLite sNoise;
};
