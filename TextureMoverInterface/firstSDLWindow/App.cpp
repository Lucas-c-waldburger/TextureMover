#include "App.h"

// APP
App::App::App()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw std::runtime_error(SDL_GetError());
    }
}

App::App::~App()
{
    TTF_Quit();
    IMG_Quit();
    SDL_VideoQuit();
    SDL_Quit();
}


// WINDOW
App::Window::Window(const char* title)
{
#if !defined(SCREEN_WIDTH) && !defined(SCREEN_HEIGHT)
    SDL_VideoInit(nullptr);
    Display display{};

    #define SCREEN_WIDTH display.WIDTH()
    #define SCREEN_HEIGHT display.HEIGHT()

    SDL_Log("SCREEN_WIDTH and SCREEN_HEIGHT global variables not defined. Window initialized using default display dimensions: %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

    pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!pWindow)
    {
        throw std::runtime_error(SDL_GetError());
    }

//#undef SCREEN_WIDTH
//#undef SCREEN_HEIGHT
}


App::Window::Window(const char* title, int w, int h, int x, int y, Uint32 flags)
{

    pWindow = SDL_CreateWindow(title, x, y, w, h, flags);

    if (!pWindow)
    {
        throw std::runtime_error(SDL_GetError());
    }
}

App::Window::~Window()
{
    SDL_DestroyWindow(pWindow);
}

App::Window::operator SDL_Window* ()
{
    return pWindow;
}


// RENDERER
App::Renderer::Renderer(SDL_Window* win)
{
    pRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!pRenderer)
    {
        throw std::runtime_error(SDL_GetError());
    }
}

App::Renderer::~Renderer()
{
    SDL_DestroyRenderer(pRenderer);
}

void App::Renderer::clear()
{
    SDL_SetRenderDrawColor(pRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(pRenderer);
}

void App::Renderer::present()
{
    SDL_RenderPresent(pRenderer);
}

App::Renderer::operator SDL_Renderer* ()
{
    return pRenderer;
}


/*---------------------------------------------- Display ----------------------------------------------*/

int App::Display::currentDisplay = 0;

App::Display::Display() : mode(), format(0)
{
    if (!init())
        throw std::runtime_error(SDL_GetError());
}

const int App::Display::WIDTH()
{
    return mode.w;
}

const int App::Display::HEIGHT()
{
    return mode.h;
}

bool App::Display::init()
{
    int displayModeCount = SDL_GetNumDisplayModes(currentDisplay);
    // SDL_Log("displayModeCount: %d", displayModeCount);
    if (displayModeCount < 1)
    {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return false;
    }

    for (int i = displayModeCount - 1; i >= 0; i--)
    {
        if (SDL_GetDisplayMode(currentDisplay, i, &mode))
        {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return false;
        }

        // SDL_Log("Display Mode #: %d   width: %d, height: %d", i, mode.w, mode.h);
    }

    format = mode.format;

    return true;
}