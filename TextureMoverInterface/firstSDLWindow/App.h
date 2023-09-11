#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdexcept>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900

namespace App
{

    class App
    {
    public:
        App();
        ~App();
    };

    class Window
    {
    public:
        Window(const char* title);
        Window(const char* title, int w, int h, int x = SDL_WINDOWPOS_UNDEFINED,
            int y = SDL_WINDOWPOS_UNDEFINED, Uint32 flags = SDL_WINDOW_SHOWN);
        ~Window();

        operator SDL_Window* ();

    private:
        SDL_Window* pWindow;
    };

    class Renderer
    {
    public:
        Renderer(SDL_Window* win);
        ~Renderer();

        void clear();
        void present();

        operator SDL_Renderer* ();

    private:
        SDL_Renderer* pRenderer;

    };

    class Display // defines SCREEN_WIDTH & SCREEN_HEIGHT based on main display resolution if not defined by user
    {
    public:
        Display();

        const int WIDTH();
        const int HEIGHT();

    private:
        static int currentDisplay;
        SDL_DisplayMode mode;
        Uint32 format;

        bool init();
    };
















};

