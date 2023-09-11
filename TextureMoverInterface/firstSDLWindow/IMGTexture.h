#pragma once
#include "BaseTexture.h"


class IMGTexture : public BaseTexture
{
public:
	IMGTexture();
	~IMGTexture();

	bool load(SDL_Renderer* renderer, const std::string& filePath) override;

	//void draw(SDL_Renderer* renderer) override;


private:



};
