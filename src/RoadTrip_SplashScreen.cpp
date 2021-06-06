#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::splashScreen_Init() {

    splashScreenVariables.reset();
    PC::buttons.pollButtons();

    this->gameState = GameState::Splash;
    this->playTheme(Themes::Intro, this->gamePlayVars.theme, this->cookie->mute, false);
}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::splashScreen() { 

    splashScreenVariables.counter++;

    if (PC::frameCount % 24 == 0) {

        splashScreenVariables.spindle++;
        
        if (splashScreenVariables.spindle == 4) {
            
            splashScreenVariables.spindle = 0;
            
        }
        
    }

    switch (this->splashScreenVariables.splashScreenState) {

        case SplashScreenState::PPOT:
            PD::drawBitmap(22, 27, Images::Ppot_Full);
            PD::drawBitmap(32, 38, Images::Ppot[splashScreenVariables.spindle]);
            if (splashScreenVariables.spindle < 2) PD::drawBitmap(81, 36, Images::Ppot_Arrow);

            if (splashScreenVariables.counter == 63) {

                this->splashScreenVariables.splashScreenState = SplashScreenState::Gibs; 

            }
            break;

        case SplashScreenState::Gibs:

            PD::drawBitmap(0, 0, Images::MG);

            if (splashScreenVariables.counter == 126) {
                this->gameState = GameState::Title_Init; 
            }
            break;

    }



}
