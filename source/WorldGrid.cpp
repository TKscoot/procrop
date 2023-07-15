#include "WorldGrid.h"

WorldGrid::WorldGrid(Player* player, Point size)
{
    mPlayer = player;
    mSize = {16, 16};

    mTileSet = new TileSheet("res/tilemap.png", {16, 16});

    CreateGrid();

    Engine::Inst()->AddEvent([&](double dt) -> void { mPlayerPos = mPlayer->GetPlayerPos(); }, EventType::UPDATE);
}

WorldGrid::~WorldGrid()
{
}

void WorldGrid::CreateGrid()
{
    // foreach chunk [3][3] create chunk object
    for (int x = -1; x <= 1; x++)
    {
        std::deque<GridChunk*> q;
        for (int y = -1; y <= 1; y++)
        {
            Point p = {x, y};
            auto c = new GridChunk(mTileSet, p, mSize, mTileScaling);
            q.push_back(c);
        }

        mChunks.push_back(q);
    }

    Debug::Warning("Chunks size x: " + std::to_string(mChunks.size()));
}

void WorldGrid::DrawGrid()
{
    Point playerChunkTilePos;
    playerChunkTilePos.x = (mPlayerPos.x / (mTileSet->GetTileDimension().x * mTileScaling.x)) % mSize.x;
    if (playerChunkTilePos.x < 0)
        playerChunkTilePos.x += 16;
    playerChunkTilePos.y = (mPlayerPos.y / (mTileSet->GetTileDimension().y * mTileScaling.y)) % mSize.y;
    if (playerChunkTilePos.y < 0)
        playerChunkTilePos.y += 16;

    Engine::Inst()->mTextRenderer->RenderText("Chunks size x: " + std::to_string(mChunks.size()), {20, 60, 100, 100});
    Engine::Inst()->mTextRenderer->RenderText("Chunks size y: " + std::to_string(mChunks.back().size()),
                                              {20, 80, 100, 100});

    Engine::Inst()->mTextRenderer->RenderText("player tile pos: {" + std::to_string(playerChunkTilePos.x) + ", " +
                                                  std::to_string(playerChunkTilePos.y) + "}",
                                              {20, 100, 100, 100});

    CheckChunkBoundaries();

    auto keysArray = SDL_GetKeyboardState(NULL);

    if (keysArray[SDL_SCANCODE_F])
    {
        // TODO: change tile based on player pos
        mChunks[1][1]->SetTileImage(playerChunkTilePos, mTileSet->GetTileImage(1));
    }

    Engine::Inst()->mTextRenderer->RenderText("current Chunks: " + mCurrentChunkIndex.ToString(), {20, 40, 100, 100});

    for (int x = 0; x < mChunks.size(); x++)
    {
        for (int y = 0; y < mChunks[x].size(); y++)
        {
            mChunks[x][y]->DrawChunk(mPlayerPos);
        }
    }
}

void WorldGrid::CheckChunkBoundaries()
{
    if (mPlayerPos.x > (mCurrentChunkIndex.x + 1) * (mSize.x * mTileSet->GetTileDimension().x * mTileScaling.x))
    {
        mCurrentChunkIndex.x++;

        // Queue with Chunk idx
        mChunks.push_back(std::deque<GridChunk*>());

        for (int y = -1; y <= 1; y++)
        {
            Point p = {mCurrentChunkIndex.x + 1, y + mCurrentChunkIndex.y};
            auto c = new GridChunk(mTileSet, p, mSize, mTileScaling);
            mChunks.back().push_back(c);
        }

        for (int i = 0; i < mChunks.front().size(); ++i)
        {
            delete mChunks.front()[i];
        }
        mChunks.pop_front();
    }
    if (mPlayerPos.y > (mCurrentChunkIndex.y + 1) * (mSize.y * mTileSet->GetTileDimension().y * mTileScaling.y))
    {
        mCurrentChunkIndex.y++;

        for (int x = -1; x <= 1; x++)
        {
            Point p = {x + mCurrentChunkIndex.x, mCurrentChunkIndex.y + 1};
            auto c = new GridChunk(mTileSet, p, mSize, mTileScaling);
            mChunks[x + 1].push_back(c);
            delete mChunks[x + 1].front();
            mChunks[x + 1].pop_front();
        }
    }
    if (mPlayerPos.x < mCurrentChunkIndex.x * (mSize.x * mTileSet->GetTileDimension().x * mTileScaling.x))
    {
        mCurrentChunkIndex.x--;

        mChunks.push_front(std::deque<GridChunk*>());

        for (int y = -1; y <= 1; y++)
        {
            Point p = {mCurrentChunkIndex.x - 1, y + mCurrentChunkIndex.y};
            auto c = new GridChunk(mTileSet, p, mSize, mTileScaling);
            mChunks.front().push_back(c);
        }

        for (int i = 0; i < mChunks.back().size(); ++i)
        {
            delete mChunks.back()[i];
        }
        mChunks.pop_back();
    }
    if (mPlayerPos.y < mCurrentChunkIndex.y * (mSize.y * mTileSet->GetTileDimension().y * mTileScaling.y))
    {
        mCurrentChunkIndex.y--;

        for (int x = -1; x <= 1; x++)
        {
            Point p = {x + mCurrentChunkIndex.x, mCurrentChunkIndex.y - 1};
            auto c = new GridChunk(mTileSet, p, mSize, mTileScaling);
            mChunks[x + 1].push_front(c);
            delete mChunks[x + 1].back();
            mChunks[x + 1].pop_back();
        }
    }
}
