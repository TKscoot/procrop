#pragma once
#include <string>
#include <SDL2/SDL_ttf.h>
#include <SDL_rect.h>

#include "Debug.h"

class TextRenderer
{
 public:
	TextRenderer(SDL_Renderer* renderer, std::string fontPath, size_t fontSize);

	void RenderText(std::string text, SDL_Rect rect, SDL_Color color = {255, 255, 255});
	
 private:
	SDL_Renderer* mRenderer = nullptr;
	
	TTF_Font* mFont = nullptr;

	SDL_Rect mRect = {0, 0, 100, 100};
	SDL_Surface* mTextSurface = nullptr;
	SDL_Texture* mTextTexture = nullptr;
};
