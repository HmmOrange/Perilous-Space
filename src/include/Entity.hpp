#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <tgmath.h>

#include "Math.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define ENTITY_PLAYER 0
#define ENTITY_STAR 1
#define ENTITY_BULLET 2
#define ENTITY_OTHER 3

class Entity{
    public: 
        Entity();
        Entity(int w, int h, float x, float y, float velX, float velY, int type, SDL_Texture *p_texture);
        float getPosX() const;
        float getPosY() const;
        float getVelX() const;
        float getVelY() const;
        int getType() const;
        Uint32 getTimeAdded() const;

        void updatePos(float p_x, float p_y);
        void updateVel(float p_velx, float p_vely);
        SDL_Texture *getTexture() const;
        SDL_Rect getCurrentFrame() const;

    protected:
        Vector2f pos;
        Vector2f vel;
        int type;
        Uint32 timeAdded;
        SDL_Rect currentFrame;
        SDL_Texture *texture;
};
