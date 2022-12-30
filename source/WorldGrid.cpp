#include "WorldGrid.h"

WorldGrid::WorldGrid(Player* player, Point size)
{
	mPlayer = player;
	mSize = { 16, 16 };

	Engine::Inst()->AddEvent([&](double dt) -> void
	{
		mPlayerPos = mPlayer->GetPlayerPos();
	}, EventType::UPDATE);

	mTileSet = new TileSheet("res/tilemap.png", { 16, 16 });

	CreateGrid();
}

WorldGrid::~WorldGrid()
{
}

void WorldGrid::CreateGrid()
{
	// foreach chunk [3][3] create chunk object
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			mChunks[x][y] = new GridChunk(mTileSet, mSize, mTileScaling);
			mChunks[x][y]->GenerateChunk({ x, y });
		}
	}
}

void WorldGrid::SetGridImage(SDL_Point index, Image* img)
{
	// dont care
}

void WorldGrid::SetGridImage(SDL_Point index, std::string imagePath)
{
	// dont care
}

void WorldGrid::DrawGrid()
{
	static Point currentChunkIndex = { 0, 0 };
	// check if player in new chunk
	// currentChunkIndex++--
	// need boolean like: updateChunk
	// create new chunk and reposition old chunks and updateChunk = false

	if (mPlayerPos.x > currentChunkIndex.x * (mSize.x * mTileSet->GetTileDimension().x * mTileScaling.x))
	{
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < 2; x++)
			{
				mChunks[x + 1][y] = mChunks[x][y];
			}
		}
		for (int y = 1; y < 3; y++)
		{
			mChunks[0][y] = new GridChunk(mTileSet, mSize, mTileScaling);
			mChunks[0][y]->GenerateChunk({ 3, y });
		}
		currentChunkIndex.x++;
	}
	if (mPlayerPos.y > currentChunkIndex.y * (mSize.y * mTileSet->GetTileDimension().y * mTileScaling.y))
	{
		currentChunkIndex.y++;
	}
	if (mPlayerPos.x < currentChunkIndex.x * (mSize.x * mTileSet->GetTileDimension().x * mTileScaling.x))
	{
		currentChunkIndex.x--;
	}
	if (mPlayerPos.y < currentChunkIndex.y * (mSize.y * mTileSet->GetTileDimension().y * mTileScaling.y))
	{
		currentChunkIndex.y--;
	}




	// draw 9 chunks (see milton drawing)
	for (int y = 0; y < 1; ++y)
	{
		for (int x = 0; x < 1; ++x)
		{
			Debug::Log("xpos: {} ypos: {}", mPlayerPos.x, mPlayerPos.y);
			auto relIdx = currentChunkIndex;
			relIdx.x += x;
			relIdx.y += y;
			//mChunks[x][y]->DrawChunk(mPlayerPos, relIdx);
		}
	}
	auto i0 = currentChunkIndex;
	i0.x -= 1;
	i0.y += 1;
	mChunks[0][0]->DrawChunk(mPlayerPos, i0);
	auto i1 = currentChunkIndex;
	i1.x -= 0;
	i1.y += 1;
	mChunks[0][1]->DrawChunk(mPlayerPos, i1);
	auto i2 = currentChunkIndex;
	i2.x += 1;
	i2.y += 1;
	mChunks[0][2]->DrawChunk(mPlayerPos, i2);
	auto i3 = currentChunkIndex;
	i3.x -= 1;
	i3.y += 0;
	mChunks[1][0]->DrawChunk(mPlayerPos, i3);
	auto i4 = currentChunkIndex;
	i4.x -= 0;
	i4.y += 0;
	mChunks[1][1]->DrawChunk(mPlayerPos, i4);
	auto i5 = currentChunkIndex;
	i5.x += 1;
	i5.y += 0;
	mChunks[1][2]->DrawChunk(mPlayerPos, i5);
	auto i6 = currentChunkIndex;
	i6.x -= 1;
	i6.y -= 1;
	mChunks[2][0]->DrawChunk(mPlayerPos, i6);
	auto i7 = currentChunkIndex;
	i7.x -= 0;
	i7.y -= 1;
	mChunks[2][1]->DrawChunk(mPlayerPos, i7);
	auto i8 = currentChunkIndex;
	i8.x += 1;
	i8.y -= 1;
	mChunks[2][2]->DrawChunk(mPlayerPos, i8);

	Engine::Inst()->mTextRenderer->RenderText(
			"ChunkIdx: " + currentChunkIndex.ToString(),
			{ 20, 40, 100, 100 });
}
