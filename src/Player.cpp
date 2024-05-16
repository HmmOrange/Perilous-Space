#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

#include "Math.hpp"
#include "Entity.hpp"
#include "InputHandler.hpp"
#include "Player.hpp"

const float GRAVITY = 0.2f;
const float JUMP_STRENGTH = -40.0f;
const float MOVE_SPEED = 0.3f;
const int GAME_INFO_OFFSET = 50;

bool Player::isOnGround(){
    return this->getPosY() >= 1.0f * (SCREEN_HEIGHT - this->getCurrentFrame().h - GAME_INFO_OFFSET);
}

bool Player::checkCollision(Entity& entity){
    int playerX1 = this->getPosX();
    int playerY1 = this->getPosY();
    int playerX2 = playerX1 + this->getCurrentFrame().w;
    int playerY2 = playerY1 + this->getCurrentFrame().h;

    int entityX1 = entity.getPosX();
    int entityY1 = entity.getPosY();
    int entityX2 = entityX1 + entity.getCurrentFrame().w;
    int entityY2 = entityY1 + entity.getCurrentFrame().h;

    return (playerX1 < entityX2) && (playerX2 > entityX1) && (playerY1 < entityY2) && (playerY2 > entityY1);
}
void Player::updateMovement(const InputHandler& inputHandler, const float& deltaTime){
    float x = this->getPosX();
    float y = this->getPosY();
    float velx = this->getVelX();
    float vely = this->getVelY();

    if (inputHandler.keyState[MOVEMENT_LEFT]) x -= MOVE_SPEED;
    if (inputHandler.keyState[MOVEMENT_RIGHT]) x += MOVE_SPEED;
    //std::cout << keyHandler.keyState[MOVEMENT_LEFT] << " " << x << " " << keyHandler.keyState[MOVEMENT_RIGHT] << " " << y << std::endl;
    if (inputHandler.keyState[MOVEMENT_UP]){
        vely = JUMP_STRENGTH;
        //std::cout << (vely + GRAVITY) << " " << (y + (vely + GRAVITY) * deltaTime) << " " << this->isOnGround() << " " << deltaTime << std::endl;
    }
    vely += GRAVITY;
    y += vely * deltaTime;

    vely = std::max(std::min(vely, 1000.0f), 0.0f);
    y = std::max(std::min(y, 1000.0f), 0.0f);

    //std::cout << "x = " << x << ", y = " << y << ", velx = " << velx << ", vely = " << vely << std::endl;
    this->updatePlayerPos(x, y);
    this->updateVel(velx, vely);

    if (this->isOnGround()){
        vely = 0;
        this->isJumping = false;
    }
}

void Player::updatePlayerPos(float x, float y) {
    // Temporarily preventing off-screen movement
    this->pos.x = std::min(std::max(0.0f, x), 1.0f * (SCREEN_WIDTH - currentFrame.w));
    this->pos.y = std::min(std::max(0.0f, y), 1.0f * (SCREEN_HEIGHT - currentFrame.h - GAME_INFO_OFFSET));

    //this->pos.x = x;
    //this->pos.y = y;
    // std::cout << y << std::endl;
}