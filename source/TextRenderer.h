#pragma once

#include <string>
#include <queue>
#include <SDL2/SDL_ttf.h>
#include <SDL_rect.h>

#include "Debug.h"

struct TextRenderData
{
	std::string text;
	SDL_Rect rect;
	SDL_Color color = { 255, 255, 255 };
};

class TextRenderer
{
public:
	TextRenderer(SDL_Renderer* renderer, std::string fontPath, size_t fontSize);

	// Only pushes text into render queue! Not rendering when called!
	void RenderText(std::string text, SDL_Rect rect, SDL_Color color = { 255, 255, 255 });

private:
	friend class Engine;
	void RenderTextQueue();

	SDL_Renderer* mRenderer = nullptr;

	TTF_Font* mFont = nullptr;

	SDL_Rect mRect = { 0, 0, 100, 100 };
	SDL_Surface* mTextSurface = nullptr;
	SDL_Texture* mTextTexture = nullptr;

	std::queue<TextRenderData> mRenderQueue = {};
};
