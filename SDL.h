#pragma once

#include <SDL2/SDL.h>
#include "colors.h"

class SDL
{
public:
    SDL();
    ~SDL();

    void destroy();
    bool screen(int width, int height, bool fullscreen = false, const std::string & text = "");
    void vertLine(int x, int y1, int y2, ColorRGB color) const;
    void render() const;
    void clear(const ColorRGB& color = RGB_Black) const;
    static bool keyDown(const Uint8* state, SDL_Keycode key);
    static const Uint8* readKeys();
    static double getTicks();
    static bool isRunning();

    typedef std::pair<int, int> IntPair;
    IntPair drawableSize() const;

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
};

