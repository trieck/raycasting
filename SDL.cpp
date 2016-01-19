#include "stdafx.h"
#include "SDL.h"

SDL::SDL() : window_(nullptr), renderer_(nullptr), texture_(nullptr)
{
}

SDL::~SDL()
{
    destroy();
    SDL_Quit();
}

void SDL::destroy()
{
    if (texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
    }

    if (renderer_ != nullptr) {
        SDL_DestroyRenderer(renderer_);
    }

    if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
    }

    texture_ = nullptr;
    renderer_ = nullptr;
    window_ = nullptr;
}

bool SDL::screen(int width, int height, bool fullscreen, const std::string& text)
{
    destroy();

    if (!fullscreen) {
        window_ = SDL_CreateWindow(text.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    } else {
        window_ = SDL_CreateWindow(text.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
            SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    if (window_ == nullptr)
        return false;

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer_ == nullptr) {
        destroy();
        return false;
    }

    if (fullscreen) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer_, width, height);
    }

    texture_ = SDL_CreateTexture(renderer_, SDL_GetWindowPixelFormat(window_), 0, width, height);
    if (texture_ == nullptr) {
        destroy();
        return false;
    }

    return true;
}

void SDL::vertLine(int x, int y1, int y2, ColorRGB color) const
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, 255);
    SDL_RenderDrawLine(renderer_, x, y1, x, y2);
}

void SDL::render() const
{
    SDL_RenderPresent(renderer_);
}

void SDL::clear(const ColorRGB& color) const
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, 255);
    SDL_RenderClear(renderer_);
}

bool SDL::keyDown(const Uint8* state, SDL_Keycode key)
{
    return (state[SDL_GetScancodeFromKey(key)] != 0);
}

const Uint8* SDL::readKeys()
{
    return SDL_GetKeyboardState(nullptr);
}

double SDL::getTicks()
{
    return SDL_GetTicks();
}

bool SDL::isRunning()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            return false;
    }

    return true;
}

SDL::IntPair SDL::drawableSize() const
{
    IntPair pair;
    SDL_GL_GetDrawableSize(window_, &pair.first, &pair.second);
    return pair;
}

