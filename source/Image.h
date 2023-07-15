#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>

#include <string>

#include "Engine.h"
#include "Debug.h"
#include "Types.h"

enum class ImageLayer{
	BACKGROUND = 0,
	MIDDLEGROUND,
	FOREGROUND
};


class Image
{
  public:
    explicit Image(std::string path);
	explicit Image(SDL_Texture* texture, bool dontDestroySdlTexture = true);

	~Image();
	
    void Load();

    void Draw();

    void Finalize();

	SDL_Texture* GetSDLTexture(){return mTexture;}

    void SetDimensions(int w, int h)
    {
        
        mRect.w = w;
        mRect.h = h;
    }

	// Comment
    void SetPosition(int x, int y)
	{
		//if (x > globalSettings.windowWidth || y > globalSettings.windowHeight)
        //{
		//	//Debug::CoreWarning("Image out of window bounds");
        //}

		mRect.x = x;
		mRect.y = y;
	}

	Point GetPosition(){return {mRect.x, mRect.y};}

    void Scale(float x, float y);

	void Flip(SDL_RendererFlip flip){mFlip = flip;}

	std::string GetPath(){return mPath;}

	Point GetDimensions(){
		SDL_Point size;
		SDL_QueryTexture(mTexture, nullptr, nullptr, &size.x, &size.y);

		return {mRect.w, mRect.h};
	}

    SDL_Rect mRect = {};
  private:

  private:
	std::string mPath = "";
    SDL_Texture *mTexture = nullptr;
	SDL_Texture *myTexturePart; 
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
	bool mDontDestroySdlTexture = false;
};
