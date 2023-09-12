# TextureMover
### Texture Animation Interface for SDL2

The TextureMover provides a simple and straightforward interface for creating step-by-step animations with SDL2 Textures. Using TextureMover requires familiarity with two main structures: the **TransformType**, and the **TextureMover** itself.

At the beginning of main(), SDL2 should be initililized through the provided **App::App** class, along with an **App::Window** and **App::Renderer** for drawing. 
Users may #define int SCREEN_WIDTH and SCREEN_HEIGHT in the App namespace to specify the window's dimensions. By default, App will create an SDL_DisplayMode instance and set the window's dimensions to the native display resolution.
```
int main() {
	App::App;
	App::Window window("Title");
	App::Renderer renderer(window);
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
Current TransformTypes supported include **Path**, **Wave**, **Rotation**, and **Flip**. See below for explanations of the different TransformTypes.
```
mover.addStep
(
Path{ LDPoint{ 0, 0 }, LDPoint{ SCREEN_WIDTH, SCREEN_HEIGHT }, 5},
Wave{ Wave::SINE, 150, 0.015 },
Rotation{ 2 },
Flip{ Flip::Type::DIAGONAL }  
);
```
Multiple animation steps may be added to a single TextureMover by calling subsequent addStep()s. 

Finally, the TextureMover is executed in the main loop by calling it's overloaded operator() between renderer.clear() and imgTexture.draw() 
```
renderer.clear();

**mover();**

imgTexture.draw(renderer);
renderer.present();
```
Multiple TextureMover objects can be created with different Textures to create full animated scenes. A built in delay system between executions is planned for the near future to support this. 
Currently, the user must add execution delays manually.

---
## TRANSFORM TYPES

### Path( _LDPoint origin, LDPoint destination, int speed, bool inPlace = false_ )
Path handles the actual movement of the texture across the screen. 
Path is the only TranformType that **MUST** be included as an argument; failing to do so will raise an invalid_argument exception.

*_LDPoint origin, LDPoint destination_ - An x and y location to mark where the texture begins and ends it's movement on screen. When adding steps beyond the first addStep(), origin may be excluded. 
					This will cause origin to be assigned the point specified in the previous step's Path.destination, useful for creating sequences of connected movements.
					**NOTE:** LDPoint is a struct derived from the standard SDL_Point that adds various logic and artithmetic operator overloads. 
 
*_int speed_ - How fast the texture will reach it's target destination.

*_bool inPlace_ - Optional parameter to carry out the texture's transformations in place. Destination must still be included, as it represents how long the step will last (equal to how long it would take for the texture to reach its' 			 destination).


### Wave( _Wave::Type type, int amplitude, float period, bool invert = false_ )
Wave modifies the texture's path to create a periodic wave movement. 

*_Wave::Type type_ - An enumerated value in the Wave struct that specifies the kind of wave function that will be applied. Currently, only Wave::SINE is supported.
 
*_int amplitude_, _float period_ - The amplitude and periodicity of the wave. Period is typically a value < 1.0.

*_bool invert_ - Optional parameter to invert the wave, applying offsets along the x-axis.


### Rotation( _int rotationSpeed, double startingAngle (optional), LDPoint center (optional)_ )
Rotation rotates textures around a center point as it travels along a Path. 

*_int rotationSpeed_ - How fast the rotation will be. Positive values for clockwise rotation, negative values for counter-clockwise, 0 for fixed.
 
*_double startingAngle_ - Optional parameter to begin the rotation at an angle different than 0 degrees (% 360).   

*_LDPoint center_ - Optional parameter to rotate the texture around a point different than its center.


### Flip( _Flip::Type type_ )
Flips the texture for the entirety of the step. 

*_Flip::Type type_ - and enumerated value representing how the texture is flipped. Currently supported types are Rotation::HORIZONTAL, Rotation::VERTICAL, Rotation::DIAGONAL, and Rotation::NONE.

---
## PRESETS (Beta)

**Preset** is a public class inside TextureMover that allows users to save created collections of steps, as well as load previously created steps into new TextureMover objects.

Saving a preset is accomplished by calling the method **.save(std::string presetName)** after adding all desired steps to the mover. 
A "presets" folder is created in the main directory along with a "presetName".txt file.

Loading a preset is done by constructing a Preset object and passing it into the overloaded TextureMover constructor (along with an IMG/TextTexture and number of repetitions).
The Preset constructor takes a single argument: the same presetName string used when saving the preset. The mover object can now be executed as usual.




 
