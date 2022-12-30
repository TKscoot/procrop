#pragma once
#include "GameObject.h"
#include "TileMap.h"

class Player : public GameObject
{
    enum PlayerFacing
    {
		BACK = 0,
		LEFT = 1,
		RIGHT = 2,
		FRONT = 3
    };

    public : Player()
        : GameObject()
    {
    }

    ~Player();

    virtual void OnStart() override;
    virtual void OnUpdate(double deltaTime) override;
    virtual void OnRender(double deltaTime) override;
    virtual void OnQuit() override;

    void UpdatePlayerMovement(double deltaTime);

	Point GetPlayerPos(){return mPlayerPos;}
	Point GetPlayerDimensions(){return mCurrentPlayerImage->GetDimensions();}

  private:
    TileSheet *mTileSheet = nullptr;
    Image *mCurrentPlayerImage = nullptr;

	bool mWalking = false;
    PointF mPlayerVel = {};
    Point mPlayerPos = {};
	PlayerFacing mFacing = PlayerFacing::FRONT;
	int mCurrentFrame = 0;
};
