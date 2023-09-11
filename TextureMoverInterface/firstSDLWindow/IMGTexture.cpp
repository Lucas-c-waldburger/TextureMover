#include "IMGTexture.h"

IMGTexture::IMGTexture()
{}

IMGTexture::~IMGTexture()
{}

bool IMGTexture::load(SDL_Renderer* renderer, const std::string& filePath)
{
    free();

    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0xFF, 0xFF));

    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    this->textureRect.w = tempSurface->w;
    this->textureRect.h = tempSurface->h;

    SDL_FreeSurface(tempSurface);

    return texture;
}

