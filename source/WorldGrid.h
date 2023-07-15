#pragma once

#include <deque>
#include "GridChunk.h"
#include "Player.h"
#include "Random.h"
#include "TileMap.h"

class WorldGrid
{
public:
	WorldGrid(Player* player, Point size);

	~WorldGrid();

	void CreateGrid();

	void DrawGrid();

private:
	void CheckChunkBoundaries();

private:
	Point mCurrentChunkIndex = { 0, 0 };

	Point mPlayerPos = { 0, 0 };
	Point mSize = { 20, 20 };
	Point mTileScaling = { 4, 4 };
	TileSheet* mTileSet = nullptr;
	Player* mPlayer = nullptr;

	std::deque<std::deque<GridChunk*>> mChunks = {};
};
