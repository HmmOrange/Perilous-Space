#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

#include "Math.hpp"
#include "Entity.hpp"
#include "InputHandler.hpp"

class Bullet: public Entity{
    using Entity::Entity;
    
    public: 
        bool isOutsidePlayground() const;
        void updateMovement();
};