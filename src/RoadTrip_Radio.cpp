#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::radio_Init() {

    this->displayClearToPattern(this->gamePlayVars.getTimeOfDay_Pattern());
    this->gamePlayVars.radioOffset = 0;
    this->gamePlayVars.radioDirection = Direction::Up;
    this->gameState = GameState::Radio;
    
}   


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::radio() {

    this->draw(true, false);

    this->renderPlayerCar();
    this->renderHud(true);
    this->renderGameOver();

    if (PC::buttons.pressed(BTN_C)) { 

        this->gamePlayVars.radioDirection = Direction::Down;

    }         

    if (this->gamePlayVars.radioOffset == 52) {
            
        if (PC::buttons.pressed(BTN_A) || PC::buttons.pressed(BTN_B)) { 

            this->gamePlayVars.radioButton = RadioButton::Off;
            this->gamePlayVars.radioButtonCount = 16;

            this->cookie->mute = !this->cookie->mute;
            this->cookie->saveCookie();

            this->playSoundEffect(SoundEffects::ButtonPress);
            this->gamePlayVars.theme = 255;
            this->playTheme(this->cookie->theme, this->gamePlayVars.theme, this->cookie->mute, true);

        }   

        if (PC::buttons.pressed(BTN_UP)) { 

            if (this->cookie->theme < 2) {

                this->gamePlayVars.radioButtonCount = 16;
                this->gamePlayVars.radioButton = RadioButton::Up;

                this->cookie->theme++;
                this->cookie->mute = false;
                this->cookie->saveCookie();

                this->playSoundEffect(SoundEffects::ButtonPress);
                this->playTheme(this->cookie->theme, this->gamePlayVars.theme, this->cookie->mute, true);

            }

        }         

        if (PC::buttons.pressed(BTN_DOWN)) { 

            if (this->cookie->theme > 0) {

                this->gamePlayVars.radioButtonCount = 16;
                this->gamePlayVars.radioButton = RadioButton::Down;

                this->cookie->theme--;
                this->cookie->mute = false;
                this->cookie->saveCookie();

                this->playSoundEffect(SoundEffects::ButtonPress);
                this->playTheme(this->cookie->theme, this->gamePlayVars.theme, this->cookie->mute, true);

            }

        }     

    }

    switch (this->gamePlayVars.radioDirection) {

        case Direction::Up:
            if (this->gamePlayVars.radioOffset < 51) this->gamePlayVars.radioOffset = this->gamePlayVars.radioOffset + 4;
            break;

        case Direction::Down:
            if (this->gamePlayVars.radioOffset > 0) this->gamePlayVars.radioOffset = this->gamePlayVars.radioOffset - 4;
            if (this->gamePlayVars.radioOffset == 0) {

                this->gameState = GameState::Game;

            }
            break;

        default: break;
    }


    PD::drawBitmap(0, 88 - this->gamePlayVars.radioOffset, Images::Radio);

    if (!this->cookie->mute) {
        PD::drawBitmap(Constants::radioPointerX[this->cookie->theme], 105 - this->gamePlayVars.radioOffset, Images::Radio_Pointer);
    }
    
    if (this->gamePlayVars.radioButtonCount > 0) {

        this->gamePlayVars.radioButtonCount--;

        switch (this->gamePlayVars.radioButton) {

            case RadioButton::Up:
                PD::drawBitmap(20, 114 - this->gamePlayVars.radioOffset, Images::Radio_Up);
                break;

            case RadioButton::Down:
                PD::drawBitmap(44, 114 - this->gamePlayVars.radioOffset, Images::Radio_Down);
                break;

            case RadioButton::Off:
                PD::drawBitmap(67, 114 - this->gamePlayVars.radioOffset, Images::Radio_Off);
                break;
        }

    }

}
