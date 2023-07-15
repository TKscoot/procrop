#include "Player.h"
#include "Debug.h"
#include "Engine.h"
#include "TileMap.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>

Player::~Player()
{
    if (mTileSheet != nullptr)
    {
        delete mTileSheet;
        mTileSheet = nullptr;
    }
}

TTF_Font *Sans; // = TTF_OpenFont("Sans.ttf", 24);
void Player::OnStart()
{
    // TTF_Init();
    // // this opens a font style and sets a size
    // Sans = TTF_OpenFont("res/fonts/OpenSans/OpenSans-Regular.ttf", 24);
    // if (!Sans)
    // {
    //     std::cout << "Error loading font: " << TTF_GetError() << std::endl;
    // }


    Debug::Log("I'm the player");
    mTileSheet = new TileSheet("res/characters/32x32/Character_010.png", {48, 48});

    for (int y = 0; y < mTileSheet->GetTileCount2D().y; y++)
    {
        for (int x = 0; x < mTileSheet->GetTileCount2D().x; x++)
        {
            // mTileSheet->GetTileImage(x, y)->SetDimensions(32, 32);
            mTileSheet->GetTileImage(x, y)->Scale(1.5f, 1.5f);
            mTileSheet->GetTileImage(x, y)->SetPosition(
                ((float)globalSettings.windowWidth / 2.0f) -
                    ((float)mTileSheet->GetTileImage(x, y)->GetDimensions().x) / 2.0f,
                ((float)globalSettings.windowHeight / 2.0f) -
                    ((float)mTileSheet->GetTileImage(x, y)->GetDimensions().y) / 2.0f);
        }
    }
    mCurrentPlayerImage = mTileSheet->GetTileImage(0, 0);

    Engine::Inst()->AddEvent(
        [&](SDL_Event &e) {
            if (e.type == SDL_KEYUP)
            {
                if (e.key.keysym.sym == SDLK_a)
                {
                    mPlayerVel.x += 1;
                    mWalking = false;
                }
                if (e.key.keysym.sym == SDLK_d)
                {
                    mPlayerVel.x -= 1;
                    mWalking = false;
                }
                if (e.key.keysym.sym == SDLK_w)
                {
                    mPlayerVel.y += 1;
                    mWalking = false;
                }
                if (e.key.keysym.sym == SDLK_s)
                {
                    mPlayerVel.y -= 1;
                    mWalking = false;
                }
            }
        },
        EventType::SDL);
}

void Player::OnUpdate(double deltaTime)
{
    UpdatePlayerMovement(deltaTime);
}

void Player::OnRender(double deltaTime)
{
    static float timer = 0.0f;

    static int mCurrentFrame = 0;
    if (mCurrentFrame > 2)
    {
        mCurrentFrame = 0;
    }

    // Debug::Log("facing: {} walking: {}", (int)mFacing, mWalking);

    mCurrentPlayerImage = mTileSheet->GetTileImage(mCurrentFrame, (int)mFacing);
    mCurrentPlayerImage->Draw();

    if (mWalking)
    {
        if (timer >= 0.175f)
        {
            mCurrentFrame++;

            timer = 0.0f;
        }
        else
        {
            timer += deltaTime;
        }
    }
    else
    {
        mCurrentFrame = 1;
    }


    // // this is the color in rgb format,
    // // maxing out all would give you the color white,
    // // and it will be your text's color
    // SDL_Color White = {255, 255, 255};

    // // as TTF_RenderText_Solid could only be used on
    // // SDL_Surface then you have to create the surface first
	std::stringstream ss;
	ss << "Player Pos:\n";
	ss << "x: ";
	ss << mPlayerPos.x;
	ss << " y: ";
	ss << mPlayerPos.y;
	Engine::Inst()->mTextRenderer->RenderText(ss.str(), {20, 20, 0, 0});
	
    // SDL_Surface *surfaceMessage = TTF_RenderText_Solid(Sans, ss.str().c_str(), White);

    // // now you can convert it into a texture
    // SDL_Texture *Message = SDL_CreateTextureFromSurface(Engine::Inst()->mRenderer, surfaceMessage);

    // SDL_Rect Message_rect; // create a rect
    // Message_rect.x = 20;
    // Message_rect.y = 20;
    // Message_rect.w = surfaceMessage->w;
    // Message_rect.h = surfaceMessage->h;
	
    // SDL_RenderCopy(Engine::Inst()->mRenderer, Message, NULL, &Message_rect);

    // // Don't forget to free your surface and texture
    // SDL_FreeSurface(surfaceMessage);
    // SDL_DestroyTexture(Message);
}

void Player::OnQuit()
{
}

void Player::UpdatePlayerMovement(double deltaTime)
{
    SDL_PumpEvents();

    // update keyboard state
    auto keysArray = SDL_GetKeyboardState(NULL);

    mPlayerVel.x = 0;
    mPlayerVel.y = 0;

    if (keysArray[SDL_SCANCODE_A])
    {
        // mPlayerVel.x -= (500.0f * deltaTime);
        mPlayerVel.x -= 1;
        mFacing = PlayerFacing::LEFT;
        mWalking = true;
    }
    else
    {
        mPlayerVel.x += 1;
    }
    if (keysArray[SDL_SCANCODE_D])
    {
        // mPlayerVel.x += 500 * deltaTime;
        mPlayerVel.x += 1;
        mFacing = PlayerFacing::RIGHT;
        mWalking = true;
    }
    else
    {
        mPlayerVel.x -= 1;
    }
    if (keysArray[SDL_SCANCODE_W])
    {
        // mPlayerVel.y -= 500 * deltaTime;
        mPlayerVel.y -= 1;
        mFacing = PlayerFacing::FRONT;
        mWalking = true;
    }
    else
    {
        mPlayerVel.y += 1;
    }
    if (keysArray[SDL_SCANCODE_S])
    {
        // mPlayerVel.y += 500 * deltaTime;
        mPlayerVel.y += 1;
        mFacing = PlayerFacing::BACK;
        mWalking = true;
    }
    else
    {
        mPlayerVel.y -= 1;
    }
    // mPlayerVel.Normalize();

    //|a| = sqrt((ax * ax) + (ay * ay) + (az * az))
    // x = ax/|a|

    float mag = SDL_sqrtf((mPlayerVel.x * mPlayerVel.x) + (mPlayerVel.y * mPlayerVel.y));

    if (mag > 0.00001f)
    {

		mPlayerVel.x = mPlayerVel.x / mag;
		mPlayerVel.y = mPlayerVel.y / mag;
    }

    mPlayerPos.x += (mPlayerVel.x * 500.0f * (float)deltaTime);
    mPlayerPos.y += (mPlayerVel.y * 500.0f * (float)deltaTime);

    // Debug::Log("x: {} y: {}", (mPlayerVel.x * 500 * deltaTime), (mPlayerVel.y * 500 * deltaTime));
}
