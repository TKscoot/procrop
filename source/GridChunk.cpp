#include "GridChunk.h"
#include <string>
#include "Debug.h"
#include "Random.h"

GridChunk::GridChunk(TileSheet *tileSheet, Point chunkSize, Point tileScaling)
{
    mTileSet = tileSheet;
    mSize = chunkSize;
    mTileScaling = tileScaling;
}

void GridChunk::GenerateChunk(Point idx)
{
    // static uint32_t seed = Random::RandomRange(0, 10000000);

    uint32_t seed = idx.x;
    seed ^= idx.y << 16;
    seed ^= idx.y >> 16;

    for (size_t y = 0; y < mSize.y; y++)
    {
        for (size_t x = 0; x < mSize.x; x++)
        {
            std::string data = std::to_string(seed) + std::to_string(x);
            std::size_t hash = std::hash<std::string>{}(data);
            data = std::to_string(seed) + std::to_string(y);
            std::size_t hash1 = std::hash<std::string>{}(data);
            uint32_t seedX = x;
            seedX ^= idx.x << 16;
            seedX ^= idx.x >> 16;
            uint32_t seedY = y;
            seedY ^= idx.y << 16;
            seedY ^= idx.y >> 16;
            auto randX = Random::RandomRange(0, mTileSet->GetTileCount2D().x, hash /*idx.y + x * seed*/, false);
            auto randY = Random::RandomRange(0, mTileSet->GetTileCount2D().y, hash1 /*idx.x + y / seed*/, false);
            // randX = 4;
            // randY = 2;

            Image *img = mTileSet->GetTileImage(randX, randY);

			if(x == 0 || x == mSize.x - 1 || y == 0 || y == mSize.y - 1){
				img = mTileSet->GetTileImage(1, 5);
			}


            Image *newImg = new Image(img->GetSDLTexture());
            newImg->SetDimensions(16, 16);
            newImg->Scale(mTileScaling.x, mTileScaling.y);
            newImg->SetPosition(mChunkIndex.x * x * mTileSet->GetTileDimension().x *
                                    mTileScaling.x, // - (mTileSet->GetTileDimension().x * mSize.x * 0.5f),
                                mChunkIndex.y * y * mTileSet->GetTileDimension().y *
                                    mTileScaling.y); // - (mTileSet->GetTileDimension().y * mSize.y * 0.5f));

            mGrid.push_back(newImg);
        }
    }
}

void GridChunk::DrawChunk(Point playerOffset, Point idx)
{
    for (size_t y = 0; y < mSize.y; y++)
    {
        for (size_t x = 0; x < mSize.x; x++)
        {
			mGrid[x * mSize.y + y]->SetPosition(
					((idx.x * mSize.x * 64) + x * mTileSet->GetTileDimension().x * mTileScaling.x) +
					(globalSettings.windowWidth / 2) -
					playerOffset.x, //- (mTileSet->GetTileDimension().x * mSize.x * mTileScaling.x * 0.5f),
					((idx.y * mSize.y * 64) + y * mTileSet->GetTileDimension().y * mTileScaling.y) +
					(globalSettings.windowHeight / 2) -
					playerOffset.y); //- (mTileSet->GetTileDimension().y * mSize.y * mTileScaling.y * 0.5f));

            mGrid[x * mSize.y + y]->Draw();
        }
    }
}
