
#include <iostream>
#include <string>
#include "App.h"
#include "IMGTexture.h"
#include "TextureMover.h"


using namespace Packs;

// baseTexture needs to read the xywh data off of the scaled rect without chang
// rendExoptions and scale stuff need to reset on reload
// 
// TODO ***** FIX SINE TABLE STUFF ***********
// CLEAN UP WHAT BELONGS WHERE 
	// create workaround to keep MovePack's TransformType members private
	// make helper functions to act as shorthand for different positions on screen
	// make TransformType Clip
		// interact with dist out to clip sprite sheets for sprite animation
	// 




Flags flags;

int main(int argc, char* argv[])
{
	// APP INIT
	App::App;
	App::Window window("Title");
	App::Renderer renderer(window);


	// TEXTURES
	IMGTexture imgTexture{};
	imgTexture.load(renderer, R"(C:\Users\Lucas\source\repos\firstSDLWindow\firstSDLWindow\square.png)");

	TextureMover::Preset testPreset{ "TEST2" };
	TextureMover mover{ &imgTexture, testPreset, TextureMover::Repeat::LOOP_FOREVER };

	std::cout << '\n' << SCREEN_WIDTH;

	//TextureMover mover{ &imgTexture, -1 };

	//mover.addStep
	//(
	//	Path{ LDPoint{ 0, SCREEN_HEIGHT / 2 - (imgTexture.getHeight() / 2)}, LDPoint{SCREEN_WIDTH - imgTexture.getWidth(), SCREEN_HEIGHT / 2 - (imgTexture.getHeight() / 2)}, 5},
	//	Wave{ Wave::SINE, 150, 0.015 }
	//);

	//mover.addStep
	//(
	//	Path{ LDPoint{SCREEN_WIDTH - imgTexture.getWidth(), SCREEN_HEIGHT / 2}, LDPoint{ SCREEN_WIDTH / 2 - (imgTexture.getWidth() / 2), 0}, 10 }
	//);

	//mover.addStep
	//( 
	//	Path{ LDPoint{ SCREEN_WIDTH / 2 - imgTexture.getWidth(), SCREEN_HEIGHT - imgTexture.getHeight() + 10}, 5},
	//	Wave{ Wave::SINE, 200, 0.011, true }
	//);

	//mover.addStep
	//(
	//	Path{ LDPoint{ 0, SCREEN_HEIGHT / 2 - imgTexture.getWidth()}, 7},
	//	Rotation{ -1.10 }
	//);

	//mover.savePreset("TEST2");



	// EVENT LOOP //
	SDL_Event e;

	/*SDL_StartTextInput();*/

	while (!flags.isSet(QUIT))
	{

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				flags.setFlag(QUIT);
			}
			//	else if (e.type == SDL_KEYDOWN)
			//	{

		}

		renderer.clear();

		// CODE GOES HERE

		mover();

		imgTexture.draw(renderer);


		renderer.present();

	}



	/*delete mousePos;*/



	return 0;
}