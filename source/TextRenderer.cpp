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
	TextRenderData rd ={text, rect, color};
	mRenderQueue.push(rd);

}

void TextRenderer::RenderTextQueue()
{
	while (!mRenderQueue.empty()) {
		auto rd = mRenderQueue.front();

		mTextSurface = TTF_RenderText_Solid(mFont, rd.text.c_str(), rd.color);
		mTextTexture = SDL_CreateTextureFromSurface(mRenderer, mTextSurface);

		rd.rect.w = mTextSurface->w;
		rd.rect.h = mTextSurface->h;

		SDL_RenderCopy(mRenderer, mTextTexture, nullptr, &rd.rect);

		SDL_FreeSurface(mTextSurface);
		SDL_DestroyTexture(mTextTexture);


		mRenderQueue.pop();
	}

}
