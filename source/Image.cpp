#include "Image.h"
#include "Debug.h"
#include "Engine.h"
#include <SDL_image.h>
#include <SDL_render.h>
#include <iostream>
#include <string>

Image::Image(std::string path)
{
	if (path.empty())
	{
		Debug::CoreError("Path to image is empty!");
	}

	mPath = path;

	Load();
	Scale(1.0f, 1.0f);
}

Image::Image(SDL_Texture* texture, bool dontDestroySdlTexture)
{
	mTexture = texture;
	mDontDestroySdlTexture = dontDestroySdlTexture;
}

Image::~Image()
{
	if (!mDontDestroySdlTexture)
	{
		if (mTexture != nullptr)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = nullptr;
		}
	}

}

void Image::Load()
{
	mTexture = IMG_LoadTexture(Engine::Inst()->mRenderer, mPath.c_str());

	if (mTexture == nullptr)
	{
		Debug::CoreError("Failed to load image: {}!\n {}", mPath, IMG_GetError());

		// create red fallback image
		auto surf = SDL_CreateRGBSurface(0, 128, 128, 1, 0xFF, 0x00, 0x00, 0xFF);
		mTexture = SDL_CreateTextureFromSurface(Engine::Inst()->mRenderer, surf);

		SDL_FreeSurface(surf);
	}
	SDL_Point size;
	SDL_QueryTexture(mTexture, nullptr, nullptr, &size.x, &size.y);
	mRect.w = size.x;
	mRect.h = size.y;
}

void Image::Scale(float x, float y)
{
	int w, h;

	SDL_QueryTexture(mTexture, nullptr, nullptr, &w, &h);

	mRect.w = w * x;
	mRect.h = h * y;
}

void Image::Draw()
{

	// SDL_RenderCopy(Engine::Inst()->mRenderer, mTexture, nullptr, &mRect);
	SDL_RenderCopyEx(Engine::Inst()->mRenderer, mTexture, nullptr, &mRect, 0.0, nullptr, mFlip);
}

void Image::Finalize()
{
	SDL_DestroyTexture(mTexture);
}
