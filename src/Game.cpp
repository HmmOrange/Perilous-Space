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

Game::Game(WindowRenderer& renderer){
    this->gameOpened = SDL_GetTicks();
    this->gameRunning = true;
    this->gameState = GAME_MAIN_MENU;
    this->bulletList.clear();
    this->starList.clear();
    this->gameScore = 0;
    this->gameHighscore = 0;
    
    if (TTF_Init() != 0){
        std::cout << "Failed to init TTF! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    this->font = TTF_OpenFont("./assets/fonts/Nunito-Bold.ttf", 20);
    std::cout << font << std::endl;
    if (this->font == NULL) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    this->mainMenu = Menu();

    SDL_Texture* titleImage = renderer.loadTexture("./assets/images/game_title_400x200.png");
    this->gameTitle = Entity(
        400, 200, 
        200, 50, 
        0, 0, 
        ENTITY_OTHER,
        titleImage
    );


    this->startButton = Button(
        320, 300, 200, 60,
        false,
        //{174, 198, 207},
        {0, 0, 255, 255},
        {253, 253, 150, 255},
        renderer,
        font,
        "Start :D"
    );
    this->mainMenu.addButton(this->startButton);

    this->quitButton = Button(
        320, 400, 200, 60,
        false,
        //{174, 198, 207},
        {0, 0, 255, 255},
        {253, 253, 150, 255},
        renderer,
        font,
        "Quit D:"
    );
    this->mainMenu.addButton(this->quitButton);
    //std::cout << this->mainMenu.getButtonList()[0].getButtonRect().x << " " << this->mainMenu.getButtonList()[0].getButtonRect().y << std::endl;
}

bool Game::isRunning() const{
    return this->gameRunning;
}

bool Game::hasClickedStart() const{
    return (this->mainMenu.getButtonList()[0]).getClickState() == IS_JUST_RELEASED;
}

int Game::getState() const{
    return this->gameState;
}

void Game::changeState(int state){
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
    this->gameState = GAME_PLAYING; 
    this->menuRendered = false;
    this->gameScore = 0;
    SDL_Texture* charImage = renderer.loadTexture("./assets/images/player_icon.png");
    this->character = Player(
        50, 50,  
        200, 200, 
        0, 0, 
        ENTITY_PLAYER, 
        charImage
    );

    if (this->starList.size() == 0){
        SDL_Texture* starImage = renderer.loadTexture("./assets/images/star_icon.png");
        Entity star(
            50, 50, 
            400, 400, 
            0, 0, 
            ENTITY_STAR,
            starImage
        );
        this->starList.push_back(star);
    }

    // Loading Infomation
    this->timeElapsedInfo = Button(
        20, 20, 100, 100,
        false, 
        {174, 198, 207, 255},
        {253, 253, 150, 255},
        renderer, 
        this->font,
        "Time: 0s"
    );

    this->scoreInfo = Button(
        300, 20, 100, 100, 
        false, 
        {174, 198, 207, 255},
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
        std::cout << "Reading highscore file...!" << std::endl;
        
        SDL_RWread(highscoreFile, &this->gameHighscore, sizeof(Sint32), 1);

        // Close file handler
        SDL_RWclose(highscoreFile);
    }

    this->highscoreInfo = Button(
        600, 20, 100, 100,
        false, 
        {174, 198, 207, 255},
        {253, 253, 150, 255},
        renderer, 
        this->font, 
        ("Highscore: " + std::to_string(this->gameHighscore)).c_str()
    );
}

void Game::processGameEvents(WindowRenderer& renderer, const InputHandler& inputHandler, const float& deltaTime){
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
                //if (bullet.isOutsidePlayground()){
                    newBulletList.push_back(bullet);   
                    if (this->character.checkCollision(bullet)){
                        // Ending the game
                        this->bulletList.clear();
                        this->starList.clear();
                        this->endGame();
                    }
                //}
                break;
        }
    }
    this->bulletList = newBulletList;

    // Update Game Infomation
    uint32_t timeElapsed = (currentTime - this->gameStartedPlaying) / 1000;
    if (timeElapsed != this->prevGameRenderTime){
        SDL_DestroyTexture(this->timeElapsedInfo.getButtonTexture());
        SDL_DestroyTexture(this->timeElapsedInfo.getTextTexture());

        // UPDATE TEXT TEXTURE FUNCTION PLEASE
        this->timeElapsedInfo = Button(
            20, 20, 100, 100,
            false,
            {174, 198, 207},
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
            300, 20, 100, 100,
            false,
            {174, 198, 207},
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
            600, 20, 100, 100,
            false,
            {174, 198, 207},
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
        SDL_SetRenderDrawColor(renderer.getRenderer(), 0, 0, 0, 255);
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

    for (auto &star: this->starList)
        renderer.render(star);

    for (auto &bullet: this->bulletList)
        renderer.render(bullet);

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

void Game::closeGame(){
    this->gameRunning = false;
}