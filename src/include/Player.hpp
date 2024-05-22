#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

#include "Math.hpp"
#include "Entity.hpp"
#include "InputHandler.hpp"

class Player: public Entity{
    using Entity::Entity;

    public: 
        Player(int w, int h, float x, float y, float velX, float velY, int type, SDL_Texture *playerTexture, SDL_Texture *shieldedTexture);

        SDL_Texture* shieldedTexture;

        bool isJumping = 0;
        bool isShielded = false;
        bool isOnGround() const;
        bool hasShield() const;
        bool checkCollision(Entity& entity);

        void updateShield(bool isShielded);
        void updateMovement(const InputHandler& keyHandler, const double& deltaTime);
        void updatePlayerPos(double x, double y);

        SDL_Texture* getShieldedTexture() const;
};