#include "TextRenderer.h"

TextRenderer::TextRenderer(SDL_Renderer* renderer, std::string fontPath, size_t fontSize)
{
	mRenderer = renderer;
	
    // Initialize SDL_ttf
    if (TTF_Init() < 0)
    {
		Debug::CoreError("Error intializing SDL_ttf: {}", TTF_GetError());
    }

	mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
	if (!mFont) {
		Debug::CoreError("Error loading font ({}): {}", TTF_GetError());
	}
}

void TextRenderer::RenderText(std::string text, SDL_Rect rect, SDL_Color color)
{
	mTextSurface = TTF_RenderText_Solid(mFont, text.c_str(), color);
	mTextTexture = SDL_CreateTextureFromSurface(mRenderer, mTextSurface);

	rect.w = mTextSurface->w;
	rect.h = mTextSurface->h;
	
	SDL_RenderCopy(mRenderer, mTextTexture, nullptr, &rect);

	SDL_FreeSurface(mTextSurface);
	SDL_DestroyTexture(mTextTexture);
}
