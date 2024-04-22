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
        bool isJumping = 0;
        bool isOnGround();
        bool checkCollision(Entity& entity);
        void updateMovement(const InputHandler& keyHandler, const float& deltaTime);
        void updatePlayerPos(float x, float y);
    
};