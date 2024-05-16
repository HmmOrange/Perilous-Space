#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <algorithm>
#include <iostream>

#include "Math.hpp"
#include "Button.hpp"
#include "WindowRenderer.hpp"

#define NOT_CLICKED 0
#define IS_BEING_HELD 1
#define JUST_RELEASED 2

Button::Button(){}

Button::Button(float x, float y, int w, int h, bool clickable, SDL_Color bgColor, SDL_Color textColor, WindowRenderer& renderer, TTF_Font* font, const char* label){
    this->clickable = clickable;
    this->textColor = textColor;
    this->bgColor = bgColor;
    this->font = font;
    this->textTexture = renderer.loadTextTexture(this->textColor, font, label);
    this->buttonTexture = SDL_CreateTexture(renderer.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    
    this->textRect = {(int)x, (int)y, 0, 0};
    SDL_QueryTexture(this->textTexture, nullptr, nullptr, &this->textRect.w, &this->textRect.h);
    this->textRect.x += (w - textRect.w) / 2;
    this->textRect.y += (h - textRect.h) / 2;

    this->buttonRect = {(int)x, (int)y, w, h};
    // std::cout << buttonRect.x << " " << buttonRect.y << std::endl;
}

bool Button::onHover(int mouseX, int mouseY) const {
    return (this->buttonRect.x <= mouseX && mouseX < this->buttonRect.x + this->buttonRect.w)
        && (this->buttonRect.y <= mouseY && mouseY < this->buttonRect.y + this->buttonRect.h);
}

SDL_Texture* Button::getTextTexture(){
    return this->textTexture;
}
SDL_Texture* Button::getButtonTexture(){
    return this->buttonTexture;
}
SDL_Color Button::getBgColor(){
    return this->bgColor;
}
SDL_Rect* Button::getTextRect(){
    return &(this->textRect);
}
SDL_Rect* Button::getButtonRect(){
    return &(this->buttonRect);
}
int Button::getClickState() const{
    return this->clickState;
}
void Button::updateClickState(int state){
    this->clickState = state;
}

void Button::updateLabel(WindowRenderer& renderer, const char* label){
    this->textTexture = renderer.loadTextTexture(this->textColor, this->font, label);
    this->textRect = {(int)this->buttonRect.x, (int)this->buttonRect.y, 0, 0};
    SDL_QueryTexture(this->textTexture, nullptr, nullptr, &this->textRect.w, &this->textRect.h);
    this->textRect.x += (this->buttonRect.w - textRect.w) / 2;
    this->textRect.y += (this->buttonRect.h - textRect.h) / 2;
}

void Button::destroyTexture(){
    SDL_DestroyTexture(this->buttonTexture);
    SDL_DestroyTexture(this->textTexture);
}