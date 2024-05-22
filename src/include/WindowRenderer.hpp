#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Entity.hpp"
#include "Player.hpp"
#include "Button.hpp"

class WindowRenderer{
    public:
        WindowRenderer(const char* p_title, int p_w, int p_h);
        SDL_Texture *loadTexture(const char* filepath);
        SDL_Texture *loadTextTexture(SDL_Color textColor, TTF_Font* font, const char* label);
        
        void clear();
        // void render(const Entity& entity);
        // void render(const Player& player);

        template <typename T>
        void render(const T& object);
        void render(const Player& player);
        void render(Button& button);
        void display();
        int getRefreshRate();
        void cleanUp();
        SDL_Renderer* getRenderer();

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
};
