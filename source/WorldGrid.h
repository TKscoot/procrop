#pragma once
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
    void SetGridImage(SDL_Point index, Image* img);
    void SetGridImage(SDL_Point index, std::string imagePath);
    void DrawGrid();

  private:
    Point mPlayerPos = {0, 0};
    Point mSize = {20, 20};
    Point mTileScaling = {4, 4};
    std::vector<Image*> mGrid = {};
    TileSheet* mTileSet = nullptr;
    Player* mPlayer = nullptr;

    std::array<std::array<GridChunk*, 3>, 3> mChunks = {};
};
