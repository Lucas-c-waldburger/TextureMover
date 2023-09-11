#include "TextTexture.h"

TextTexture::TextTexture(const std::string& filePath, int size) : font(nullptr), fontColor({ 0xFF, 0xFF, 0xFF, 0xFF }), fontSize(size)
{
    font = TTF_OpenFont(filePath.c_str(), size);
}

TextTexture::~TextTexture()
{
    TTF_CloseFont(font);
    font = nullptr;
}

bool TextTexture::load(SDL_Renderer* renderer, const std::string& filePath)
{
    free();

    SDL_Surface* tempSurface = TTF_RenderUTF8_Blended(font, currentText.c_str(), fontColor);
    texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    this->textureRect.w = tempSurface->w;
    this->textureRect.h = tempSurface->h;

    SDL_FreeSurface(tempSurface);

    return texture;
}

SDL_Color TextTexture::getFontColor() const
{
    return fontColor;
}

void TextTexture::setFontColor(SDL_Color c)
{
    fontColor = c;
}


