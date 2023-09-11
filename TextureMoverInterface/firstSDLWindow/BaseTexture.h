#pragma once
#include "App.h"
#include "utils.h"

using namespace Utils;

class BaseTexture
{
public:
	BaseTexture();
	virtual ~BaseTexture();

	virtual bool load(SDL_Renderer* renderer, const std::string& filePath) = 0;
	virtual void draw(SDL_Renderer* renderer);
	virtual void free();

	int getWidth() const;
	int getHeight() const;

	int getX() const;
	void setX(int newX);

	int getY() const;
	void setY(int newY);

	LDPoint getXY() const;
	void setXY(LDPoint newXY);

	LDPoint getCenter() const;

	void setColorMod(SDL_Color c);
	void setAlphaMod(Uint8 a);
	void setBlendMode(SDL_BlendMode blending);

	void resetRendEx();

	class RenderEXOptions
	{
	public:
		RenderEXOptions();
		RenderEXOptions(SDL_Rect& srcRect);
		~RenderEXOptions();

		void setRotationAngle(double newAng);
		const double getRotationAngle();
		void setRotationCenter(LDPoint newCent);
		const LDPoint getRotationCenter();
		void setFlipType(SDL_RendererFlip newFlipType);
		const SDL_RendererFlip getFlipType();
		void setRenderRect(const SDL_Rect& newRect);


		
		bool centerNull();

		friend class BaseTexture;

	private:
		SDL_Rect* renderRect;
		double rotationAngle;
		LDPoint* rotationCenter;
		SDL_RendererFlip SDLFlipType;
	};

	RenderEXOptions& getRendExOptions();

	friend class TextureMover;

protected:
	SDL_Texture* texture;
	SDL_Rect textureRect;
	RenderEXOptions rendExOptions;
};

