#pragma once

#include <SDL2/SDL.h>
#include <SDl2/SDL_image.h>
#include <chrono>
#include <random>

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

namespace utils{
    inline double getTimeInSeconds(){
        double tick = SDL_GetTicks();
        tick *= 0.001;
        return tick;
    }

    inline int random(int l, int r){
        return rng() % (r - l + 1) + l;
    }
}   