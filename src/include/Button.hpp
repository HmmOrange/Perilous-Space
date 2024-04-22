#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#include "Math.hpp"

#define IS_NOT_CLICKED 0
#define IS_BEING_HELD 1
#define IS_JUST_RELEASED 2

class WindowRenderer;

class Button{
    public: 
        Button();
        Button(float x, float y, int w, int h, bool clickable, SDL_Color bgColor, SDL_Color textColor, WindowRenderer& renderer, TTF_Font* font, const char* label);
        float getPosX() const;
        float getPosY() const;

        SDL_Texture* getTextTexture();
        SDL_Texture* getButtonTexture();
        SDL_Color getBgColor();
        const SDL_Rect* getTextRect();
        const SDL_Rect* getButtonRect();

        int getClickState() const;
        void updateClickState(int state);
        void updateLabel(WindowRenderer& renderer, const  char* label);
        void destroyTexture();
        bool onHover(int mouseX, int mouseY) const;
    
    private:
        bool clickable;
        int clickState = IS_NOT_CLICKED;
        bool hasJustReleased = false;
        TTF_Font* font;
        SDL_Color bgColor;
        SDL_Color textColor;
        SDL_Texture *textTexture;
        SDL_Texture *buttonTexture;
        SDL_Rect textRect;
        SDL_Rect buttonRect;
};
