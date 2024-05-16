#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <math.h>
#include <string>

#include "Entity.hpp"
#include "Player.hpp"
#include "Bullet.hpp"
#include "WindowRenderer.hpp"
#include "Button.hpp"

WindowRenderer::WindowRenderer(const char* p_title, int p_w, int p_h): window(NULL), renderer(NULL){
	this->window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL){
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture *WindowRenderer::loadTexture(const char* p_filepath){
    SDL_Texture *texture = IMG_LoadTexture(renderer, p_filepath);
    if (texture == NULL){
        std::cout << "SDL_Texture Load Failed: " << SDL_GetError() << std::endl;
    }
    return texture;
} 

SDL_Texture *WindowRenderer::loadTextTexture(SDL_Color textColor, TTF_Font* font, const char* label){
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, label, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);
    
    if (textTexture == NULL){
        std::cout << "SDL_Texture Load Failed: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);
    return textTexture;
}

void WindowRenderer::clear(){
    SDL_RenderClear(renderer);
}

// void WindowRenderer::render(const Entity& entity){
//     SDL_Rect src;
//     src.x = entity.getCurrentFrame().x;
//     src.y = entity.getCurrentFrame().y;
//     src.w = entity.getCurrentFrame().w;
//     src.h = entity.getCurrentFrame().h;

//     SDL_Rect dst;
//     dst.x = (int)round(entity.getPosX());
//     dst.y = (int)round(entity.getPosY());
//     dst.w = entity.getCurrentFrame().w;
//     dst.h = entity.getCurrentFrame().h;
//     SDL_RenderCopy(this->renderer, entity.getTexture(), &src, &dst);
// }


// void WindowRenderer::render(const Player& player){
//     SDL_Rect src;
//     src.x = player.getCurrentFrame().x;
//     src.y = player.getCurrentFrame().y;
//     src.w = player.getCurrentFrame().w;
//     src.h = player.getCurrentFrame().h;

//     SDL_Rect dst;
//     dst.x = (int)round(player.getPosX());
//     dst.y = (int)round(player.getPosY());
//     dst.w = player.getCurrentFrame().w;
//     dst.h = player.getCurrentFrame().h;
//     SDL_RenderCopy(this->renderer, player.getTexture(), &src, &dst);
// }


template void WindowRenderer::render<Entity>(const Entity& object);
template void WindowRenderer::render<Bullet>(const Bullet& object);
template void WindowRenderer::render<Player>(const Player& object);

template <typename T>
void WindowRenderer::render(const T& object){
    SDL_Rect src;
    src.x = object.getCurrentFrame().x;
    src.y = object.getCurrentFrame().y;
    src.w = object.getCurrentFrame().w;
    src.h = object.getCurrentFrame().h;

    SDL_Rect dst;
    dst.x = (int)round(object.getPosX());
    dst.y = (int)round(object.getPosY());
    dst.w = object.getCurrentFrame().w;
    dst.h = object.getCurrentFrame().h;
    SDL_RenderCopy(this->renderer, object.getTexture(), &src, &dst);
}

void WindowRenderer::render(Button& button){ 
    SDL_RenderCopy(this->renderer, button.getButtonTexture(), nullptr, button.getButtonRect());
    SDL_SetRenderDrawColor(this->renderer, button.getBgColor().r, button.getBgColor().g, button.getBgColor().b, button.getBgColor().a);
    SDL_RenderFillRect(this->renderer, button.getButtonRect());
    
    SDL_RenderCopy(this->renderer, button.getTextTexture(), nullptr, button.getTextRect());
    //std::cout << button.getButtonRect()->x << " " << button.getButtonRect()->y << std::endl;

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255); // Black color
}


void WindowRenderer::display(){
    SDL_RenderPresent(this->renderer);
}

int WindowRenderer::getRefreshRate(){
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);

    return mode.refresh_rate;
}

SDL_Renderer* WindowRenderer::getRenderer(){
    return this->renderer;
}
void WindowRenderer::cleanUp(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}




