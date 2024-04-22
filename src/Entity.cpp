#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>

#include "Math.hpp"
#include "Entity.hpp"

Entity::Entity(){}

Entity::Entity(int w, int h, float x, float y, float velX, float velY, int type, SDL_Texture *p_texture){
    this->pos.x = x;
    this->pos.y = y;
    this->vel.x = velX;
    this->vel.y = velY;
    this->texture = p_texture;
    this->type = type;
    this->timeAdded = SDL_GetTicks();
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = (int)round(w);
    currentFrame.h = (int)round(h);
}

float Entity::getPosX() const {
    return this->pos.x;
}

float Entity::getPosY() const {
    return this->pos.y;
}

float Entity::getVelX() const {
    return this->vel.x;
}

float Entity::getVelY() const {
    return this->vel.y;
}

int Entity::getType() const {
    return this->type;
}

Uint32 Entity::getTimeAdded() const {
    return this->timeAdded;
}

void Entity::updatePos(float x, float y){
    this->pos.x = x;
    this->pos.y = y;

    //if (this->type == ENTITY_STAR) std::cout << this->pos.x << " " << this->pos.y << std::endl;
}

void Entity::updateVel(float velx, float vely){
    this->vel.x = velx;
    this->vel.y = vely;
}

SDL_Texture* Entity::getTexture() const {
    return texture;
}

SDL_Rect Entity::getCurrentFrame() const {
    return currentFrame;
}