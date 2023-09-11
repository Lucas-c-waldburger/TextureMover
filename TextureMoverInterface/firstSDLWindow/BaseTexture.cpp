#include "BaseTexture.h"


/*---------------------------------------------- BaseTexture ----------------------------------------------*/

BaseTexture::BaseTexture() : texture(nullptr), textureRect({ 0, 0, 0, 0 })
{}

BaseTexture::~BaseTexture()
{
    free();
}

void BaseTexture::draw(SDL_Renderer* renderer)
{
    SDL_Rect tempRect = (!rendExOptions.renderRect) ? this->textureRect : *rendExOptions.renderRect;
    /*std::cout << "WIDTH: " << tempRect.w << ", HEIGHT: " << tempRect.h << '\n'
        << "X: " << tempRect.x << ", Y: " << tempRect.y << '\n';*/
    SDL_RenderCopyEx(renderer, texture, nullptr, &tempRect, rendExOptions.rotationAngle, rendExOptions.rotationCenter, rendExOptions.SDLFlipType);
}

void BaseTexture::free()
{
    if (texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        this->textureRect.w = this->textureRect.h = 0;
    }
}

int BaseTexture::getWidth() const
{
    return this->textureRect.w;
}

int BaseTexture::getHeight() const
{
    return this->textureRect.h;
}

int BaseTexture::getX() const
{
    return this->textureRect.x;
}

void BaseTexture::setX(int newX)
{
    this->textureRect.x = newX;
}

int BaseTexture::getY() const
{
    return this->textureRect.y;
}

void BaseTexture::setY(int newY)
{
    this->textureRect.y = newY;
}

LDPoint BaseTexture::getXY() const
{
    return LDPoint(this->textureRect.x, this->textureRect.y);
}

void BaseTexture::setXY(LDPoint newXY)
{
    this->textureRect.x = newXY.x; this->textureRect.y = newXY.y;
}

LDPoint BaseTexture::getCenter() const
{
    return LDPoint{ textureRect.x + (textureRect.w / 2), textureRect.y + (textureRect.h / 2) };
}

void BaseTexture::setColorMod(SDL_Color c)
{
    SDL_SetTextureColorMod(texture, c.r, c.g, c.b);
}

void BaseTexture::setAlphaMod(Uint8 a)
{
    SDL_SetTextureAlphaMod(texture, a);
}

void BaseTexture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(texture, blending);
}

void BaseTexture::resetRendEx()
{
    rendExOptions.renderRect = &textureRect;

    delete rendExOptions.rotationCenter;
    rendExOptions.rotationCenter = nullptr;

    rendExOptions.rotationAngle = 0.0;
    rendExOptions.SDLFlipType = SDL_FLIP_NONE;
}

BaseTexture::RenderEXOptions& BaseTexture::getRendExOptions()
{
    return this->rendExOptions;
}


/*---------------------------------------------- RenderExOptions ----------------------------------------------*/

BaseTexture::RenderEXOptions::RenderEXOptions() : renderRect(nullptr), rotationCenter(nullptr), rotationAngle(0.), SDLFlipType(SDL_FLIP_NONE)
{}

BaseTexture::RenderEXOptions::RenderEXOptions(SDL_Rect& srcRect) : renderRect(&srcRect), rotationCenter(nullptr), rotationAngle(0.), SDLFlipType(SDL_FLIP_NONE)
{}

BaseTexture::RenderEXOptions::~RenderEXOptions()
{
    renderRect = nullptr;
    delete rotationCenter;
}

void BaseTexture::RenderEXOptions::setRotationAngle(double newAng)
{
    this->rotationAngle = newAng;
}

const double BaseTexture::RenderEXOptions::getRotationAngle()
{
    return this->rotationAngle;
}

void BaseTexture::RenderEXOptions::setRotationCenter(LDPoint newCent)
{
    delete rotationCenter;
    rotationCenter = new LDPoint(newCent);
}

const LDPoint BaseTexture::RenderEXOptions::getRotationCenter()
{
    LDPoint rotationCenterCpy(*rotationCenter);
    return rotationCenterCpy;
}

void BaseTexture::RenderEXOptions::setFlipType(SDL_RendererFlip newFlipType)
{
    SDLFlipType = newFlipType;
}

const SDL_RendererFlip BaseTexture::RenderEXOptions::getFlipType()
{
    return SDLFlipType;
}

void BaseTexture::RenderEXOptions::setRenderRect(const SDL_Rect& newRect)
{
    delete renderRect;
    this->renderRect = new SDL_Rect(newRect);
}

bool BaseTexture::RenderEXOptions::centerNull()
{
    return rotationCenter == nullptr;
}
