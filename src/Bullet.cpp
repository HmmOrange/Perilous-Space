#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

#include "Math.hpp"
#include "Entity.hpp"
#include "KeyboardHandler.hpp"
#include "Bullet.hpp"

bool Bullet::isOutsidePlayground() const {
    bool down = this->getPosY() > SCREEN_HEIGHT;
    bool up = this->getPosY() < 0 - this->getCurrentFrame().h;
    bool left = this->getPosX() < 0 - this->getCurrentFrame().w;
    bool right = this->getPosY() > SCREEN_WIDTH;

    return down || up || left || right;
}


void Bullet::updateMovement(){
    this->updatePos(this->getPosX() + this->getVelX(), this->getPosY() + this->getVelY());
}