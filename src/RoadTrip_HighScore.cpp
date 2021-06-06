#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;



// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::HighScore_Init() {

    this->cameraPos.setX(0);
    this->cameraPos.setY(UPM);
    this->cameraPos.setZ(-40);

    this->world.reset();
    this->gamePlayVars.reset();
    this->gameState = GameState::HighScore;

    this->highScoreVars.cursor = 0;

    placeRoadSide();
    
}   


// --------------------------------------------------------------------------------------
//  Render High Score screen ..
//
void Game::renderHighScore() {

    uint32_t index = cookie->getScoreIndex();

    uint8_t xOffset = 25;
    uint8_t yOffset = 26;

    draw(false, true);

    PD::setColor(BLACK);
    PD::fillRect(14, 12, 82, 64);
    PD::setColor(WHITE);
    PD::drawFastVLine(15, 24, 51);
    PD::drawFastVLine(94, 24, 51);
    PD::drawFastHLine(16, 75, 78);

    PD::drawBitmap(7, 6, Images::HighScore[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())]);

    switch (this->gamePlayVars.getGameLevel()) {

        case GameLevel::Auto:
            PD::drawBitmap(28, 78, Images::Automatic);
            break;

        case GameLevel::Manual:
            PD::drawBitmap(28, 78, Images::Manual);
            break;

    }


    

    for (uint8_t x = 0; x < Constants::MaxNumberOfScores; x++) {

        PD::drawBitmap(xOffset, yOffset, Images::Font4x6_White[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][x][0]]);
        PD::drawBitmap(xOffset + 6, yOffset, Images::Font4x6_White[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][x][1]]);
        PD::drawBitmap(xOffset + 12, yOffset, Images::Font4x6_White[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][x][2]]);


        // Score ..
        {
            uint8_t digits[6] = {};
            extractDigits(digits, cookie->score[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][x]);

            for (uint8_t i = 0, x2 = xOffset + 54; i < 6; ++i, x2 -= 5) { 
                PD::drawBitmap(x2, yOffset, Images::Font4x6_White[digits[i] + 26]);
            }

        }

        yOffset = yOffset + 10;

    }

    if (index != NO_SCORE) {

        yOffset = 26;
        highScoreVars.alternate++;

        if (highScoreVars.alternate < 24) {

            PD::drawBitmap(xOffset - 5, yOffset + (index * 10) - 1, Images::Arrow);
            PD::drawBitmap(xOffset + 62, yOffset + (index * 10) - 1, Images::Arrow);

        }
        else if (highScoreVars.alternate > 48) {

            highScoreVars.alternate = 0;

        }

        // Handle buttons ..

        if (PC::buttons.pressed(BTN_LEFT) && this->highScoreVars.cursor > 0) {
            this->highScoreVars.cursor--;
        }

        if (PC::buttons.pressed(BTN_RIGHT) && this->highScoreVars.cursor < 2) {
            this->highScoreVars.cursor++;
        }

        if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, 16)) {

            uint8_t val = cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][this->highScoreVars.cursor];
            val++;
            if (val > 25) val = 0;
            cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][this->highScoreVars.cursor] = val;
            
        }

        if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.repeat(BTN_DOWN, 16)) {

            uint8_t val = cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][this->highScoreVars.cursor];
            val--;
            if (val == 255) val = 25;
            cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][this->highScoreVars.cursor] = val;
            
        }

        if (PC::buttons.pressed(BTN_A)) {

            if (cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][0] != 0 && cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][1] != 0 && cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][2] != 0) {

                cookie->saveCookie();
                cookie->setLastScore(this->gamePlayVars.getGameLevel(), 0);

            }
            else if (this->highScoreVars.cursor < 2 && cookie->initials[index][this->highScoreVars.cursor] != 0 ) {
                this->highScoreVars.cursor++;
            }
            
        }

        if (PC::buttons.pressed(BTN_A)) { 

            cookie->setScoreIndex(NO_SCORE);

        }

        if (this->highScoreVars.cursor != 0) { 
            PD::drawBitmap(xOffset, yOffset + (index * 10), Images::Font4x6_White[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][0]]); 
        }
        else {
            PD::setColor(Constants::ColourWhite);
            PD::fillRect(xOffset - 1, yOffset + (index * 10) - 1, 6, 7);
            PD::drawBitmap(xOffset, yOffset + (index * 10), Images::Font4x6_Black[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][0]]); 
        }

        if (this->highScoreVars.cursor != 1) { 
            PD::drawBitmap(xOffset + 6, yOffset + (index * 10), Images::Font4x6_White[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][1]]); 
        }
        else {
            PD::setColor(Constants::ColourWhite);
            PD::fillRect(xOffset + 5, yOffset + (index * 10) - 1, 6, 7);
            PD::drawBitmap(xOffset + 6, yOffset + (index * 10), Images::Font4x6_Black[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][1]]); 
        }

        if (this->highScoreVars.cursor != 2) { 
            PD::drawBitmap(xOffset + 12, yOffset + (index * 10), Images::Font4x6_White[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][2]]); 
        }
        else {
            PD::setColor(Constants::ColourWhite);
            PD::fillRect(xOffset + 11, yOffset + (index * 10) - 1, 6, 7);
            PD::drawBitmap(xOffset + 12, yOffset + (index * 10), Images::Font4x6_Black[this->cookie->initials[static_cast<uint8_t>(this->gamePlayVars.getGameLevel())][index][2]]); 
        }

    }
    else {


       // Clear scores ..

        if ((PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, REPEAT_VAL)) && (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, REPEAT_VAL))) {

            highScoreVars.clearScores++;

            if (highScoreVars.clearScores - 61) {

                highScoreVars.clearScores = 0;
                this->cookie->initialise();
                cookie->saveCookie();
                cookie->setLastScore(this->gamePlayVars.getGameLevel(),0);                
                return;

            }

        }
        else {

            if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.pressed(BTN_RIGHT)) {

                switch (this->gamePlayVars.getGameLevel()) {

                    case GameLevel::Auto:
                        this->gamePlayVars.setGameLevel(GameLevel::Manual);
                        break;

                    case GameLevel::Manual:
                        this->gamePlayVars.setGameLevel(GameLevel::Auto);
                        break;

                }

            }


            if (highScoreVars.clearScores > 0) {

                highScoreVars.clearScores = 0;

            }

        }

        if (PC::buttons.pressed(BTN_A)) { 

            this->gameState = GameState::Title_Init; 

        }

    }


    houseKeeping();
    moveRoadSide();
    
    this->cameraPos.setZ(this->cameraPos.getZ() + 6);
    this->cameraPos.setY(this->world.roadHeightAt(this->cameraPos.getZ() + 2 * UPM) + UPM);
    this->gamePlayVars.setTime(1700);

}
