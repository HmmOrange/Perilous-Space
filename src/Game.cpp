#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <vector>

#include "Game.hpp"
#include "Utils.hpp"
#include "WindowRenderer.hpp"

#define SCREEN_UP 0
#define SCREEN_DOWN 1
#define SCREEN_LEFT 2
#define SCREEN_RIGHT 3
#define BULLET_SPAWNING_TIME 1

#define TIME_ELAPSED_INFO_X 25
#define TIME_ELAPSED_INFO_Y 562
#define TIME_ELAPSED_INFO_W 100
#define TIME_ELAPSED_INFO_H 25

#define SCORE_INFO_X 290
#define SCORE_INFO_Y 562
#define SCORE_INFO_W 100
#define SCORE_INFO_H 25

#define HIGHSCORE_INFO_X 580
#define HIGHSCORE_INFO_Y 562
#define HIGHSCORE_INFO_W 100
#define HIGHSCORE_INFO_H 25

#define PLACEHOLDER_X 0
#define PLACEHOLDER_Y 550
#define PLACEHOLDER_W 800
#define PLACEHOLDER_H 50

#define BUTTON_BACKGROUND_R 0
#define BUTTON_BACKGROUND_G 0
#define BUTTON_BACKGROUND_B 139
#define BUTTON_BACKGROUND_A 255

#define PLAYER_W 30
#define PLAYER_H 30

#define STAR_W 40
#define STAR_H 40

#define PLAYER_IMAGE_PATH "./assets/images/player_square.png"
#define STAR_IMAGE_PATH "./assets/images/star.png"
#define SHIELDED_PLAYER_IMAGE_PATH "./assets/images/shielded_player_square.png"
#define SHIELD_IMAGE_PATH "./assets/images/shield_icon.png"
#define SHIELD_TIMER_IMAGE_PATH "./assets/images/shield_timer_circle.png"
#define BUTTON_BACKGROUND_IMAGE_PATH "./assets/images/button.png"
#define GAME_TITLE_IMAGE_PATH "./assets/images/game_title_400x200.png"
#define STAR_COLLECT_SFX_PATH "./assets/audio/star_collect.wav"
#define SHIELD_COLLECT_SFX_PATH "./assets/audio/shield_collect.wav"
#define JUMP_SFX_PATH "./assets/audio/jump.ogg"
#define BULLET_COLLISION_SFX_PATH "./assets/audio/bullet_collision.wav"

Game::Game(WindowRenderer& renderer){
    this->gameOpened = SDL_GetTicks();
    this->gameState = GAME_MAIN_MENU;
    this->bulletList.clear();
    this->starList.clear();
    this->shieldList.clear();
    this->gameScore = 0;
    this->gameHighscore = 0;
    this->shieldTimerCircleTexture = renderer.loadTexture(SHIELD_TIMER_IMAGE_PATH);

    if (TTF_Init() != 0){
        std::cout << "Failed to init TTF! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    this->font = TTF_OpenFont("./assets/fonts/Nunito-Bold.ttf", 20);
    if (this->font == NULL) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    // if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    //     std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    // }

    // if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    //     std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
    // }
    
    // if (Mix_Init(MIX_INIT_WAVPACK) & MIX_INIT_WAVPACK != MIX_INIT_WAVPACK){
    //     std::cout << "Failed to initialize OGG" << " " << Mix_GetError() << std::endl;
    // }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout <<  "Failed to initialize SDL_mixer! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

    this->starCollectSFX = Mix_LoadWAV(STAR_COLLECT_SFX_PATH);
    if (!this->starCollectSFX) {
        std::cout << "Failed to load star WAV file: " << Mix_GetError() << std::endl;
    }

    this->shieldCollectSFX = Mix_LoadWAV(SHIELD_COLLECT_SFX_PATH);
    if (!this->shieldCollectSFX) {
        std::cout << "Failed to load shield WAV file: " << Mix_GetError() << std::endl;
    }
    
    this->bulletCollisionSFX = Mix_LoadWAV(BULLET_COLLISION_SFX_PATH);
    if (!this->shieldCollectSFX) {
        std::cout << "Failed to load shield WAV file: " << Mix_GetError() << std::endl;
    }

    // this->jumpSFX = Mix_LoadWAV(JUMP_SFX_PATH);
    // if (!this->jumpSFX) {
    //     std::cout << "Failed to load star OGG file: " << Mix_GetError() << std::endl;
    // }

    this->mainMenu = Menu();

    SDL_Texture* titleImage = renderer.loadTexture(GAME_TITLE_IMAGE_PATH);
    this->gameTitle = Entity(
        400, 200, 
        200, 50, 
        0, 0, 
        ENTITY_OTHER,
        titleImage
    );

    SDL_Texture* buttonBackgroundImage = renderer.loadTexture(BUTTON_BACKGROUND_IMAGE_PATH);
    this->startButton = Button(
        320, 300, 200, 60,
        false,
        buttonBackgroundImage,
        {253, 253, 150, 255},
        renderer,
        font,
        "Start :D"
    );
    this->mainMenu.addButton(this->startButton);

    this->quitButton = Button(
        320, 400, 200, 60,
        false,
        buttonBackgroundImage,
        {253, 253, 150, 255},
        renderer,
        font,
        "Quit D:"
    );
    this->mainMenu.addButton(this->quitButton);
    //std::cout << this->mainMenu.getButtonList()[0].getButtonRect().x << " " << this->mainMenu.getButtonList()[0].getButtonRect().y << std::endl;
}

bool Game::isRunning() const{
    return this->gameState != GAME_QUIT;
}

bool Game::hasClickedStart() const{
    //std::cout << this->mainMenu.getButtonList().size() << std::endl;
    return (this->mainMenu.getButtonList()[0]).getClickState() == IS_JUST_RELEASED;
}

bool Game::hasClickedQuit() const{
    return (this->mainMenu.getButtonList()[1]).getClickState() == IS_JUST_RELEASED;
}

int Game::getState() const{
    return this->gameState;
}

void Game::updateState(int state){
    this->gameState = state;
}

void Game::addStar(const Entity& star){
    this->starList.push_back(star);
}

void Game::addBullet(const Bullet& bullet){
    this->bulletList.push_back(bullet);
}

std::vector<Entity> Game::getStarList() const{
    return this->starList;
}

std::vector<Bullet> Game::getBulletList() const{
    return this->bulletList;
}

Player Game::getPlayer() const {
    return this->character;
}

void Game::startNewGame(WindowRenderer& renderer){
    this->gameStartedPlaying = SDL_GetTicks();
    this->lastShieldTime = SDL_GetTicks();
    this->gameState = GAME_PLAYING; 
    this->menuRendered = false;
    this->gameScore = 0;
    this->bulletList.clear();
    this->starList.clear();
    this->shieldList.clear();

    SDL_Texture* charImage = renderer.loadTexture(PLAYER_IMAGE_PATH);
    SDL_Texture* shieldedImage = renderer.loadTexture(SHIELDED_PLAYER_IMAGE_PATH);
    this->character = Player(
        PLAYER_W, PLAYER_H, 
        200, 200, 
        0, 0, 
        ENTITY_PLAYER, 
        charImage, shieldedImage
    );

    if (this->starList.size() == 0){
        SDL_Texture* starImage = renderer.loadTexture(STAR_IMAGE_PATH);
        Entity star(
            STAR_W, STAR_H, 
            400, 400, 
            0, 0, 
            ENTITY_STAR,
            starImage
        );
        this->starList.push_back(star);
    }

    // Loading Infomation
    this->timeElapsedInfo = Button(
        TIME_ELAPSED_INFO_X, TIME_ELAPSED_INFO_Y, TIME_ELAPSED_INFO_W, TIME_ELAPSED_INFO_H,
        false, 
        {BUTTON_BACKGROUND_R, BUTTON_BACKGROUND_G, BUTTON_BACKGROUND_B, BUTTON_BACKGROUND_A},
        {253, 253, 150, 255},
        renderer, 
        this->font,
        "Time: 0s"
    );

    this->scoreInfo = Button(
        SCORE_INFO_X, SCORE_INFO_Y, SCORE_INFO_W, SCORE_INFO_H,
        false, 
        {BUTTON_BACKGROUND_R, BUTTON_BACKGROUND_G, BUTTON_BACKGROUND_B, BUTTON_BACKGROUND_A},
        {253, 253, 150, 255},
        renderer,
        this->font,
        "Score: 0"
    );

    SDL_RWops* highscoreFile = SDL_RWFromFile("./local/data/highscore.bin", "r+b");
    if (highscoreFile == NULL){
        std::cout << "Warning: Unable to open file! SDL Error: " << SDL_GetError() << std::endl;
        
        // Create file for writing
        highscoreFile = SDL_RWFromFile("./local/data/highscore.bin", "w+b");
        if (highscoreFile != NULL){
            std::cout << "New file created!" << std::endl;

            // Initialize data    
            SDL_RWwrite(highscoreFile, &this->gameHighscore, sizeof(Sint32), 1);
            
            // Close file handler
            SDL_RWclose(highscoreFile);
        }
        else{
            std::cout << "Error: Unable to create file! SDL Error: " << SDL_GetError() << std::endl;
        }
        this->gameHighscore = 0;
    }
    else{
        // Load data
        //std::cout << "Reading highscore file...!" << std::endl;
        
        SDL_RWread(highscoreFile, &this->gameHighscore, sizeof(Sint32), 1);

        // Close file handler
        SDL_RWclose(highscoreFile);
    }

    this->highscoreInfo = Button(
        HIGHSCORE_INFO_X, HIGHSCORE_INFO_Y, HIGHSCORE_INFO_W, HIGHSCORE_INFO_H,
        false, 
        {BUTTON_BACKGROUND_R, BUTTON_BACKGROUND_G, BUTTON_BACKGROUND_B, BUTTON_BACKGROUND_A},
        {253, 253, 150, 255},
        renderer, 
        this->font, 
        ("Highscore: " + std::to_string(this->gameHighscore)).c_str()
    );
    SDL_Texture* infoPlaceholderImage = renderer.loadTexture("./assets/images/info_placeholder.png");
    this->infoPlaceholder = Entity(
        PLACEHOLDER_W, PLACEHOLDER_H,  
        PLACEHOLDER_X, PLACEHOLDER_Y,
        0, 0, 
        ENTITY_OTHER, 
        infoPlaceholderImage
    );
}

void Game::processGameEvents(WindowRenderer& renderer, const InputHandler& inputHandler, const double& deltaTime){
    // Update player moment based on key presses
    // if (this->font)
    //     std::cout << "###" << this->font << std::endl;
    this->character.updateMovement(inputHandler, deltaTime);

    // Check and add new bullets
    uint32_t currentTime = SDL_GetTicks();
    double timePassed = (currentTime - this->gameStartedPlaying) / 1000.0;
    double bulletSpawnTime = 2 - sqrt(std::min(120.0, timePassed) / 41.5);

    // Exponentially reduce spawn time until 2 minutes
    // Spawn rate: 2 -> 0.3

    if (!this->bulletList.size() || (currentTime - (*this->bulletList.rbegin()).getTimeAdded()) / 1000.0 >= bulletSpawnTime){
        int side = utils::random(0, 3);
        Bullet bullet;
        SDL_Texture* bullet30 = renderer.loadTexture("./assets/images/bullet30x30.png");

        switch (side){
            case SCREEN_UP:
                bullet = Bullet(
                    30, 30,
                    utils::random(100, SCREEN_WIDTH - 100), 0 - 30,
                    utils::random(-25, 25)/100.0, utils::random(15, 25)/100.0,
                    ENTITY_BULLET, 
                    bullet30
                );

                break;

            // case SCREEN_DOWN:
            //     bullet = Bullet(
            //         30, 30,
            //         utils::random(100, SCREEN_WIDTH - 100), SCREEN_HEIGHT,
            //         utils::random(-25, 25)/100.0, utils::random(-25, -15)/100.0,
            //         ENTITY_BULLET, 
            //         bullet30
            //     );
            //     break;
            
            case SCREEN_LEFT:
                bullet = Bullet(
                    30, 30,
                    0 - 30, utils::random(100, SCREEN_WIDTH - 100),
                    utils::random(15, 25)/100.0, utils::random(-25, 25)/100.0,
                    ENTITY_BULLET, 
                    bullet30
                );

                break;

            case SCREEN_RIGHT:
                bullet = Bullet(
                    30, 30,
                    SCREEN_WIDTH, utils::random(100, SCREEN_WIDTH - 100),
                    utils::random(-25, -15)/100.0, utils::random(-25, 25)/100.0,
                    ENTITY_BULLET, 
                    bullet30
                );
                break;
        }
        // std::cout << bullet.getPosX() << " " << bullet.getPosY() << std::endl;
        this->bulletList.push_back(bullet);
    }

    // Spawn shield every 20 second
    if ((currentTime - this->lastShieldTime) / 1000.0 >= 20){
        SDL_Texture* shieldImage = renderer.loadTexture(SHIELD_IMAGE_PATH);
       
        Entity shield(
            30, 30, 
            utils::random(100, 700), utils::random(100, 500), 
            0, 0, 
            ENTITY_SHIELD,
            shieldImage
        );

        this->shieldList.push_back(shield);
        this->lastShieldTime = currentTime;
    }

    // Update star upon collision and bullets' movement
    bool newScore = false;
    for (Entity &star: this->starList){
       // bool check = false;
        if (star.getType() == ENTITY_STAR){
            if (this->character.checkCollision(star)){
                    //check = true;
                    //std::cout << "Before change:" << star.getPosX() << std::endl;
                    star.updatePos(utils::random(100, 700), utils::random(100, 500));
                    //std::cout << "After change:" << star.getPosX() << std::endl;
                    this->gameScore++;
                    Mix_PlayChannel(-1, this->starCollectSFX, 0);
                    //std::cout << Mix_PlayMusic(this->starCollectSFX, 0) << std::endl;
                    // if (Mix_PlayChannel(-1, this->starCollectSFX, 0) == -1) {  // 0 means play once
                    //     std::cout << "Failed to play star sound effect: " << Mix_GetError() << std::endl;
                    // }
                    newScore = true;
             }
        }
        //if (check) std::cout << "After if:" << star.getPosX() << std::endl;
    }

    std::vector<Bullet> newBulletList;
    for (Bullet &bullet: this->bulletList){    
        switch (bullet.getType()){
            case ENTITY_BULLET:
                bullet.updateMovement();
                bool deleteBullet = false;
                //if (bullet.isOutsidePlayground()){
                    if (this->character.checkCollision(bullet)){
                        Mix_PlayChannel(-1, this->bulletCollisionSFX, 0);
                        if (this->character.hasShield()){
                            this->character.updateShield(false);
                            deleteBullet = true;
                            break;
                        }
                        // Ending the game
                        this->endGame();
                        break;
                    }
                    if (!deleteBullet) newBulletList.push_back(bullet);   
                //}
                break;
        }
        if (this->gameState == GAME_END) break;
    }
    this->bulletList = newBulletList;

    std::vector<Entity> newShieldList;
    for (Entity &shield: this->shieldList){    
        switch (shield.getType()){
            case ENTITY_SHIELD:
                if (this->character.checkCollision(shield)){
                    this->character.updateShield(true);
                    Mix_PlayChannel(-1, this->shieldCollectSFX, 0);
                    break;
                }
                else 
                    newShieldList.push_back(shield);
        }
    }
    this->shieldList = newShieldList;

    // Update Game Infomation

    uint32_t timeElapsed = (currentTime - this->gameStartedPlaying) / 1000 ;
    if (timeElapsed != this->prevGameRenderTime){
        SDL_DestroyTexture(this->timeElapsedInfo.getButtonTexture());
        SDL_DestroyTexture(this->timeElapsedInfo.getTextTexture());

        // UPDATE TEXT TEXTURE FUNCTION PLEASE
        this->timeElapsedInfo = Button(
            TIME_ELAPSED_INFO_X, TIME_ELAPSED_INFO_Y, TIME_ELAPSED_INFO_W, TIME_ELAPSED_INFO_H,
            false,
            {BUTTON_BACKGROUND_R, BUTTON_BACKGROUND_G, BUTTON_BACKGROUND_B, BUTTON_BACKGROUND_A},
            {253, 253, 150},
            renderer,
            this->font,
            ("Time: " + std::to_string(timeElapsed) + "s").c_str()
        );
        this->prevGameRenderTime = timeElapsed;
    }

    if (newScore){
        SDL_DestroyTexture(this->scoreInfo.getButtonTexture());
        SDL_DestroyTexture(this->scoreInfo.getTextTexture());

        this->scoreInfo = Button(
            SCORE_INFO_X, SCORE_INFO_Y, SCORE_INFO_W, SCORE_INFO_H,
            false,
            {BUTTON_BACKGROUND_R, BUTTON_BACKGROUND_G, BUTTON_BACKGROUND_B, BUTTON_BACKGROUND_A},
            {253, 253, 150},
            renderer,
            this->font,
            ("Score: " + std::to_string(this->gameScore)).c_str()
        );
    }

    if (this->gameHighscore < this->gameScore){
        this->gameHighscore = this->gameScore;
        SDL_DestroyTexture(this->highscoreInfo.getButtonTexture());
        SDL_DestroyTexture(this->highscoreInfo.getTextTexture());
        this->highscoreInfo = Button(
            HIGHSCORE_INFO_X, HIGHSCORE_INFO_Y, HIGHSCORE_INFO_W, HIGHSCORE_INFO_H,
            false,
            {BUTTON_BACKGROUND_R, BUTTON_BACKGROUND_G, BUTTON_BACKGROUND_B, BUTTON_BACKGROUND_A},
            {253, 253, 150},
            renderer,
            this->font,
            ("Highscore: " + std::to_string(this->gameHighscore)).c_str()
        );
    }
}

Menu* Game::getMainMenu(){
    return &this->mainMenu;
}
void Game::renderMainMenu(WindowRenderer& renderer){
    if (this->menuRendered == false){
        SDL_SetRenderDrawColor(renderer.getRenderer(), 118, 2, 137, 0);
        renderer.clear();
        this->menuRendered = true;
    }
    
    //std::cout << "Size: " << this->mainMenu.getButtonList().size() << std::endl;
    renderer.render(this->gameTitle);
    for (auto &button: this->mainMenu.getButtonList()){
        //std::cout << "1" << " - " << (int)button.getBgColor().r << " " << (int)button.getBgColor().g << " " << (int)button.getBgColor().b << " " << (int)button.getBgColor().a << std::endl;
        renderer.render(button);
    }

    //std::cout << "3" << std::endl;
    renderer.display();
}

void Game::renderPlaying(WindowRenderer& renderer){
    renderer.clear();
    renderer.render(this->character);

    bool removeShield = false;
    for (auto &shield: this->shieldList){
        uint32_t currentTime = SDL_GetTicks();
        double shieldTimePassed = (currentTime - shield.getTimeAdded()) / 1000.0;
        if (shieldTimePassed > 5){
            removeShield = true;
            break;
        }
        

        Entity shieldTimerCircle(
            50, 50, 
            shield.getPosX() - 10, shield.getPosY() - 10, 
            0, 0, 
            ENTITY_OTHER,
            this->shieldTimerCircleTexture
        );

        renderer.render(shieldTimerCircle);
        utils::drawPie(
            renderer,
            shield.getPosX() + 15, shield.getPosY() + 15,
            22, 
            0, 360 / 5 * shieldTimePassed
        );
        renderer.render(shield);
    }

    if (removeShield) this->shieldList.clear();

    for (auto &star: this->starList)
        renderer.render(star);

    for (auto &bullet: this->bulletList)
        renderer.render(bullet);

    
    renderer.render(this->infoPlaceholder);
    renderer.render(this->timeElapsedInfo);
    renderer.render(this->scoreInfo);
    renderer.render(this->highscoreInfo);
    renderer.display();

}
void Game::endGame(){
    this->gameState = GAME_END;
    SDL_RWops* file = SDL_RWFromFile("./local/data/highscore.bin", "w+b");
    if (file != NULL){
        // Save data
        SDL_RWwrite(file, &this->gameHighscore, sizeof(Sint32), 1);

        // Close file handler
        SDL_RWclose(file);
    }
    else{
        std::cout << "Error: Unable to save file! " << SDL_GetError() << std::endl;
    }
}