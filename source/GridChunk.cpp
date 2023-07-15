#include "GridChunk.h"
#include <string>
#include <future>
#include <limits>

#include "Debug.h"

FastNoiseLite GridChunk::sNoise = {};

GridChunk::GridChunk(TileSheet* tileSheet, Point chunkPosition, Point chunkSize, Point tileScaling)
{
	mTileSet = tileSheet;
	mSize = chunkSize;
	mTileScaling = tileScaling;
	mChunkPosition = chunkPosition;

	mVegetationTileSet = new TileSheet("res/vegetation/vegetation_spritesheet.png", { 64, 64 });

	//std::async(std::launch::async, [this](){
	//	GenerateChunk();
	//});
	std::async(std::launch::async, std::bind(&GridChunk::GenerateChunk, this));
	//GenerateChunk();
}

int64_t hash(int x, int cx)
{
	uint64_t result = uint16_t(cx);
	result = (result << 16) + uint16_t(x);
	return result;
}

void GridChunk::GenerateChunk()
{
	// static uint32_t seed = Random::RandomRange(0, 10000000);
	for (auto& it: mGrid)
	{
		for (auto& img: it.second)
		{
			if (img)
			{
				delete img;
				img = nullptr;
			}
		}
		it.second.clear();
	}


	uint32_t seed = mChunkPosition.x;
	seed ^= mChunkPosition.y << 16;
	seed ^= mChunkPosition.y >> 16;

	for (int y = 0; y < mSize.y; y++)
	{
		for (int x = 0; x < mSize.x; x++)
		{
			std::string data = std::to_string(seed) + std::to_string(x);
			std::size_t hashX = std::hash<std::string>{}(data);
			data = std::to_string(seed) + std::to_string(y);
			std::size_t hashY = std::hash<std::string>{}(data);

			srand(hashX);
			auto randX = rand() % mTileSet->GetTileCount2D().x;
			srand(hashY);
			auto randY = rand() % mTileSet->GetTileCount2D().y;

			//auto randX = Random::RandomRange(0, mTileSet->GetTileCount2D().x, hashX, false);
			//auto randY = Random::RandomRange(0, mTileSet->GetTileCount2D().y, hashY, false);

			SetBackgroundSprite(x, y);
		}
	}
}

void GridChunk::DrawChunk(Point playerOffset)
{
	Point tilePixelSize = { mTileSet->GetTileDimension().x * mTileScaling.x,
							mTileSet->GetTileDimension().y * mTileScaling.y };

	for (size_t y = 0; y < mSize.y; y++)
	{
		for (size_t x = 0; x < mSize.x; x++)
		{

			for (Image* image : mGrid[ImageLayer::BACKGROUND])
			{

				image->SetPosition(
						((mChunkPosition.x * mSize.x * tilePixelSize.x) + x * tilePixelSize.x) +
						(globalSettings.windowWidth / 2) - playerOffset.x,
						((mChunkPosition.y * mSize.y * tilePixelSize.y) + y * tilePixelSize.y) +
						(globalSettings.windowHeight / 2) - playerOffset.y);
				image->Draw();
			}

			/*
			for (auto& img: mGrid[ImageLayer::MIDDLEGROUND])
			{

				img->SetPosition(
						((mChunkPosition.x * mSize.x ) + x ) +
						(globalSettings.windowWidth / 2) - playerOffset.x,
						((mChunkPosition.y * mSize.y ) + y ) +
						(globalSettings.windowHeight / 2) - playerOffset.y);
				img->Draw();
			}
			*/

		}
	}

	SDL_Rect r;
	r.w = 100;
	r.h = 100;
	r.x = ((mChunkPosition.x * mSize.x * mTileSet->GetTileDimension().x * mTileScaling.x) +
		   (mTileSet->GetTileDimension().x * mSize.x * mTileScaling.x) / 2) +
		  (globalSettings.windowWidth / 2) - playerOffset.x;
	r.y = ((mChunkPosition.y * mSize.y * mTileSet->GetTileDimension().y * mTileScaling.y) +
		   (mTileSet->GetTileDimension().y * mSize.y * mTileScaling.y) / 2) +
		  (globalSettings.windowHeight / 2) - playerOffset.y;

	Engine::Inst()->mTextRenderer->RenderText(
			mChunkPosition.ToString(),
			r, { 0, 0, 255 });
}

void GridChunk::SetTileImage(Point pos, Image* img, ImageLayer layer)
{
	if (!img)
	{
		Debug::CoreError("Image is null");
		return;
	}

	if (pos.x * mSize.y + pos.y > mGrid.size())
	{
		Debug::CoreError("Index out of range");
		//return;
	}

	delete mGrid[layer][pos.x * mSize.y + pos.y];    // TODO: Potentially buggy because we might delete SDL_Surface*
	mGrid[layer][pos.x * mSize.y + pos.y] = nullptr;

	Image* newImg = new Image(img->GetSDLTexture());
	newImg->SetDimensions(16, 16);
	newImg->Scale(mTileScaling.x, mTileScaling.y);
	newImg->SetPosition(mChunkPosition.x * pos.x * mTileSet->GetTileDimension().x * mTileScaling.x,
			mChunkPosition.y * pos.y * mTileSet->GetTileDimension().y * mTileScaling.y);

	mGrid[layer][pos.x * mSize.y + pos.y] = newImg;
}

void GridChunk::SetBackgroundSprite(int x, int y)
{
	sNoise.SetSeed(time(nullptr));
	sNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	float noise = sNoise.GetNoise((float)(x * mChunkPosition.x), (float)(y * mChunkPosition.y));
	Image* img = nullptr;

	ranged_value_t<float, Point> table[] = {    // TODO: Table in wrong scope! Move to somewhere else. Maybe as const LUT member.
			{{ -1.0f, 0.3f }, Point(1, 1) },
			{{ 0.3f,  1.0f }, Point(5, 1) }
	};

	Point imgIndex = { 3, 7 };

	try
	{
		imgIndex = *find_value(std::begin(table), std::end(table), noise);
	}
	catch (std::out_of_range& e)
	{
		Debug::CoreError("Out of range: " + std::string(e.what()));
		imgIndex = { 3, 7 };
	}

	img = mTileSet->GetTileImage(imgIndex.x, imgIndex.y);

	auto* newImg = new Image(img->GetSDLTexture());
	newImg->SetDimensions(16, 16);
	newImg->Scale(mTileScaling.x, mTileScaling.y);
	newImg->SetPosition(mChunkPosition.x * x * mTileSet->GetTileDimension().x * mTileScaling.x,
			mChunkPosition.y * y * mTileSet->GetTileDimension().y * mTileScaling.y);

	mGrid[ImageLayer::BACKGROUND].push_back(newImg);

	Image* vegimg = mVegetationTileSet->GetTileImage(1, 0);

	auto* newVegImg = new Image(vegimg->GetSDLTexture());
	//newVegImg->SetDimensions(16, 16);
	//newVegImg->Scale(mTileScaling.x, mTileScaling.y);
	newVegImg->SetPosition(mChunkPosition.x * x * mVegetationTileSet->GetTileDimension().x,
			mChunkPosition.y * y * mVegetationTileSet->GetTileDimension().y );

	mGrid[ImageLayer::MIDDLEGROUND].push_back(newVegImg);
}

