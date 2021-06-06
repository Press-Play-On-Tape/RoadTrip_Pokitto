#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

#include "utils/Constants.h"
#include "utils/Utils.h"
#include "utils/Structs.h"
#include "entities/Entities.h"
#include "images/Images.h"

void Game::setup(GameCookie *cookie) {

    this->cookie = cookie;

}

void Game::loop() {

    PD::clear();
    PC::buttons.pollButtons();

    switch (this->gameState) {

        case GameState::Splash_Init:

            splashScreen_Init();
            splashScreen();
            break;

        case GameState::Splash:

            splashScreen();
            break;

        case GameState::Title_Init:

            PD::fillScreen(8);
            title_Init();
            title();
            break;

        case GameState::Title:

            this->displayClearToPattern(this->gamePlayVars.getTimeOfDay_Pattern());
            title();
            break;

        case GameState::Game_Init:

            game_Init();
            game();
            break;

        case GameState::Game:
            this->displayClearToPattern(this->gamePlayVars.getTimeOfDay_Pattern());
            game();
            break;

        case GameState::GameOver_Init:

            this->displayClearToPattern(this->gamePlayVars.getTimeOfDay_Pattern());
            gameOver_Init();
            gameOver();
            break;            

        case GameState::GameOver:

            this->displayClearToPattern(this->gamePlayVars.getTimeOfDay_Pattern());
            gameOver();
            break;

        case GameState::SaveScore:

            cookie->setLastScore(this->gamePlayVars.getGameLevel(), this->gamePlayVars.distance);
            if (this->cookie->getScoreIndex() != NO_SCORE) {
                this->playSoundEffect_FromSD(SoundEffects::Winner, false);
            }
            this->gameState = GameState::HighScore_Init;
            this->playTheme(Themes::HighScore, this->gamePlayVars.theme, this->cookie->mute, false);
            [[fallthrough]];
 
        case GameState::HighScore_Init:

            PC::buttons.pollButtons();
            HighScore_Init();
            [[fallthrough]];

        case GameState::HighScore:

            renderHighScore();
            break;

        case GameState::Radio_Init:

            PC::buttons.pollButtons();
            radio_Init();
            [[fallthrough]];

        case GameState::Radio:

            this->displayClearToPattern(this->gamePlayVars.getTimeOfDay_Pattern());
            radio();
            break;            

    }

}

