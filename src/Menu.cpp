#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "Menu.hpp"
#include "Button.hpp"
#include "Math.hpp"

#define MOUSE_UP 0
#define MOUSE_DOWN 1

Menu::Menu(){}

void Menu::addButton(Button& button){
    this->buttonList.push_back(button);
}

void Menu::updateButtonState(int mouseX, int mouseY, int mouseState){
    for (auto &button: this->buttonList){
        if (button.onHover(mouseX, mouseY) && mouseState == MOUSE_DOWN){
            button.updateClickState(IS_BEING_HELD);
        }
        else if (button.onHover(mouseX, mouseY) && mouseState == MOUSE_UP){
            if (button.getClickState() == IS_BEING_HELD){
                button.updateClickState(IS_JUST_RELEASED);
            }
            else{
                button.updateClickState(IS_NOT_CLICKED);
            }
        }
        else if (!button.onHover(mouseX, mouseY) && mouseState == MOUSE_UP){
            button.updateClickState(IS_NOT_CLICKED);
        }
    }
    
    if (this->buttonList[0].getClickState() == IS_JUST_RELEASED) 
        std::cout << this->buttonList[0].getClickState() << std::endl;
}
std::vector<Button> Menu::getButtonList() const {
    return this->buttonList;
}