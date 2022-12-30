#pragma once
#include "TileMap.h"
#include "Random.h"
#include "Types.h"

class GridChunk
{
  public:
	GridChunk(TileSheet* tileSheet, Point chunkSize, Point tileScaling);

	void GenerateChunk(Point idx);
	void DrawChunk(Point playerOffset, Point idx);
	
  private:
	Point mChunkIndex = {0,0};
	Point mPosition = {0, 0};
    Point mSize = {20, 20};
    Point mTileScaling = {4, 4};
    std::vector<Image *> mGrid = {};
	TileSheet* mTileSet = nullptr;
};
