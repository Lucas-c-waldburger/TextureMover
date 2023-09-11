#pragma once
#include "BaseTexture.h"

// Text Mover Class will be in charge of telling it's textTexture when it needs to load in a new Surface-To-Texture
class TextTexture : public BaseTexture
{
public:
	TextTexture(const std::string& filePath, int size);
	~TextTexture();

	bool load(SDL_Renderer* renderer, const std::string& filePath) override;
	//void draw(SDL_Renderer* renderer) override;

	SDL_Color getFontColor() const;
	void setFontColor(SDL_Color c);

private:
	TTF_Font* font;
	SDL_Color fontColor;
	int fontSize;
	std::string currentText;

};

