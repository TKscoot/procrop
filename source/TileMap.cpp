#include "TileMap.h"
#include "Debug.h"
#include "Image.h"
#include <SDL_render.h>
#include <immintrin.h>

SDL_Texture *GetAreaTextrue(SDL_Rect rect, SDL_Renderer *renderer, SDL_Texture *source, uint32_t format)
{
    SDL_Texture *result = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, result);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(source, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, source, &rect, NULL);
    // the folowing line should reset the target to default(the screen)
    SDL_SetRenderTarget(renderer, NULL);
    // I also removed the RenderPresent funcion as it is not needed here
    SDL_RenderPresent(renderer);
    return result;
}

TileSheet::TileSheet(std::string filepath, SDL_Point tileSize)
{
    mTileSize = tileSize;
    Image *img = new Image(filepath);

    int w, h;
    uint32_t format;
    SDL_QueryTexture(img->GetSDLTexture(), &format, nullptr, &w, &h);

    mSourceSize.x = w;
    mSourceSize.y = h;

    CutTilesToTextures(img->GetSDLTexture(), format);
}

void TileSheet::CutTilesToTextures(SDL_Texture *tex, uint32_t format)
{
    for (size_t x = 0; x < (mSourceSize.x / mTileSize.x); x++)
    {
        for (size_t y = 0; y < (mSourceSize.y / mTileSize.y); y++)
        {
            SDL_Rect r;
            r.x = x * mTileSize.x;
            r.y = y * mTileSize.y;
            r.w = mTileSize.x;
            r.h = mTileSize.y;

            auto tileTex = GetAreaTextrue(r, Engine::Inst()->mRenderer, tex, format);

            Image *tileImg = new Image(tileTex);
            r.x = 0;
            r.y = 0;
            r.w = tileImg->GetDimensions().x;
            r.h = tileImg->GetDimensions().y;

            tileImg->mRect = r;

            mTiles.push_back(tileImg);
        }
    }
}
