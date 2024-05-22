#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>

#include "Math.hpp"
#include "Entity.hpp"
#include "InputHandler.hpp"
#include "Player.hpp"

const double GRAVITY = 0.2f;
const double JUMP_STRENGTH = -40.0f;
const double MOVE_SPEED = 0.3f;
const int GAME_INFO_OFFSET = 50;

Player::Player(int w, int h, float x, float y, float velX, float velY, int type, SDL_Texture *texture, SDL_Texture *shieldedTexture){
    this->pos.x = x;
    this->pos.y = y;
    this->vel.x = velX;
    this->vel.y = velY;
    this->texture = texture;
    this->shieldedTexture = shieldedTexture;
    this->type = type;
    this->timeAdded = SDL_GetTicks();
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = (int)round(w);
    currentFrame.h = (int)round(h);
}

bool Player::isOnGround() const {
    return this->getPosY() >= 1.0f * (SCREEN_HEIGHT - this->getCurrentFrame().h - GAME_INFO_OFFSET);
}

bool Player::hasShield() const {
    return this->isShielded;
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

void Player::updateShield(bool isShielded){
    this->isShielded = isShielded;
}

void Player::updateMovement(const InputHandler& inputHandler, const double& deltaTime){
    double x = this->getPosX();
    double y = this->getPosY();
    double velx = this->getVelX();
    double vely = this->getVelY();
    //std::cout << velx << " " << vely << std::endl;

    if (inputHandler.keyState[MOVEMENT_LEFT]) x -= MOVE_SPEED;
    if (inputHandler.keyState[MOVEMENT_RIGHT]) x += MOVE_SPEED;
    //std::cout << keyHandler.keyState[MOVEMENT_LEFT] << " " << x << " " << keyHandler.keyState[MOVEMENT_RIGHT] << " " << y << std::endl;
    if (inputHandler.keyState[MOVEMENT_UP]){
        vely = JUMP_STRENGTH;
        //std::cout << (vely + GRAVITY) << " " << (y + (vely + GRAVITY) * deltaTime) << " " << this->isOnGround() << " " << deltaTime << std::endl;
    }
    vely += GRAVITY;
    y += vely * deltaTime;

    vely = std::max(std::min(vely, 100.0), 0.0);
    y = std::max(std::min(y, 1000.0), 0.0);

    // std::cout << "x = " << x << ", y = " << y << ", velx = " << velx << ", vely = " << vely << std::endl;
    this->updatePlayerPos(x, y);
    this->updateVel(velx, vely);

    if (this->isOnGround()){
        vely = 0;
        this->isJumping = false;
    }
}

void Player::updatePlayerPos(double x, double y) {
    // Temporarily preventing off-screen movement
    this->pos.x = std::min(std::max(0.0, x), 1.0 * (SCREEN_WIDTH - currentFrame.w));
    this->pos.y = std::min(std::max(0.0, y), 1.0 * (SCREEN_HEIGHT - currentFrame.h - GAME_INFO_OFFSET));

    //this->pos.x = x;
    //this->pos.y = y;
    // std::cout << y << std::endl;
}

SDL_Texture* Player::getShieldedTexture() const {
    return this->shieldedTexture;
}