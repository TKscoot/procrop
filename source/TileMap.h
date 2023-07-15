#pragma once

#include "Image.h"
#include "Debug.h"
#include <SDL_rect.h>

class TileSheet
{
public:
	TileSheet(std::string filepath, SDL_Point tileSize);

	Image* GetTileImage(int x, int y)
	{
		return mTiles[x * GetTileCount2D().y + y];
	}

	Image* GetTileImage(int index)
	{
		if (index >= mTiles.size())
		{
			Debug::CoreError("Index out of range!");
			return nullptr;
		}

		return mTiles[index];
	}

	SDL_Point GetTileCount2D()
	{
		SDL_Point p;
		p.x = (mSourceSize.x / mTileSize.x);
		p.y = (mSourceSize.y / mTileSize.y);

		return p;
	}

	size_t GetTileCount1D()
	{
		return (mSourceSize.x / mTileSize.x) * (mSourceSize.y / mTileSize.y);
	}

	SDL_Point GetTileDimension()
	{
		return mTileSize;
	}

private:
	void CutTilesToTextures(SDL_Texture* tex, uint32_t format);

private:
	SDL_Point mSourceSize = {};
	SDL_Point mTileSize = {};
	std::vector<Image*> mTiles;
};
