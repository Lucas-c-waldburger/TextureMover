# TextureMover
### Texture Animation Interface for SDL2

The TextureMover provides a simple and straightforward interface for creating step-by-step animations with SDL2 Textures.

Using TextureMover requires familiarity with two main structures: the TransformType, and the TextureMover itself.

At the beginning of main(), SDL2 should be initililized through the provided **App::App** class, along with an **App::Window** and **App::Renderer** for drawing. 
Users may #define int SCREEN_WIDTH and SCREEN_HEIGHT in the App namespace to specify the window's dimensions. By default, App will create an SDL_DisplayMode instance and set the window's dimensions to the native display resolution.
```
int main() {
	App::App;
	App::Window window("Title");
	App::Renderer renderer(window);
  ...
```

Next, the user creates an SDL_Texture* by default-constructing one of the provided wrapper classes: **IMGTexture** (for SDL_IMG textures) or **TextTexture** (SDL_TTF textures). The **.load()** method is then called with the App::Renderer and a 
path name to the desired image as its arguments.
```
IMGTexture imgTexture{};
imgTexture.load(renderer, "square.png");
```

Now it's time to create the TextureMover object. TextureMover's constructor takes two arguments: the address of the IMG/TextTexture and an integer specifying how many times to repeat the final animation (0 for no repetitions, 
-1 or the enumerated value "TextureMover::Repeat::LOOP_FOREVER" for infinite) 
_NOTE: TextureMover may be constructed with a Preset object instead, which will be covered later._
```
TextureMover mover{ &imgTexture, TextureMover::Repeat::LOOP_FOREVER };
```

From this point, discreet animation steps are added to the TextureMover through the **.addStep()** method. addStep takes a variable number of arguments of types derived from the base "TransformType" struct. These objects represent different 
movements and transformations that can be applied to the texture, and are instantiated directly within the addStep function call. 
Current TransformTypes supported include **Path**, **Wave**, **Rotation**, and **Flip**.
```
mover.addStep
(
Path{ LDPoint{ 0, 0 }, LDPoint{ SCREEN_WIDTH, SCREEN_HEIGHT }, 5},
Wave{ Wave::SINE, 150, 0.015 },
Rotation{ 2 },
Flip{ Flip::Type::DIAGONAL }  
);
```

###PATH( _LDPoint origin, LDPoint destination, int speed, bool inPlace )_





