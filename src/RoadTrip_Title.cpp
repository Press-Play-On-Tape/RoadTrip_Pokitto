#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::title_Init() {
    
    this->cameraPos.setX(0);
    this->cameraPos.setY(UPM);
    this->cameraPos.setZ(-40);

    this->world.reset();
    this->gamePlayVars.reset();
    titleVars.reset();

    this->gameState = GameState::Title;

    placeRoadSide();

    #ifdef DEBUG_COLLISIONS
        titleVars.offset = Constants::Title_MaxOffset - 3;
    #endif

}   


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::title() {

    // Update position of text ..

    if (titleVars.flyIn && titleVars.offset < Constants::Title_MaxOffset) {

        titleVars.offset = titleVars.offset + 3;

    }

    if (!titleVars.flyIn) {

        if (titleVars.offset > 0) {
            titleVars.offset = titleVars.offset - 3;
        }
        else {

            switch (titleVars.cursor) {
        
                case 0:
                    if (titleVars.seenAim)  {
                        titleVars.titleState = TitleState::SelectTransmission;
                        this->playSoundEffect_FromSD(this->car.getTransmissionType() == TransmissionType::Auto ? SoundEffects::Auto : SoundEffects::Manual, false);
                    }
                    else {
                        titleVars.titleState = TitleState::Aim;
                        titleVars.seenAim = true;
                        this->playSoundEffect_FromSD(SoundEffects::Gentlemen, false);
                    }
                    titleVars.flyIn = true;
                    break;
        
                case 1:
                    this->gamePlayVars.setGameLevel(PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, REPEAT_VAL) ? GameLevel::Easy : this->car.getTransmissionType() == TransmissionType::Auto ? GameLevel::Auto : GameLevel::Manual); 
                    this->cookie->setScoreIndex(NO_SCORE);
                    this->gameState = GameState::HighScore_Init;
                    break;

            }

        }

    }


    draw(false, true);

    switch (titleVars.titleState) {

        case TitleState::StartGame:

            PD::drawBitmap(DISPLAY_WIDTH - titleVars.offset, 14, Images::RoadTrip_00);
            PD::drawBitmap(-69 + titleVars.offset, 40, Images::RoadTrip_01);
        
            if (titleVars.offset == Constants::Title_MaxOffset) {

                if (PC::buttons.pressed(BTN_A)) { 

                    titleVars.flyIn = false;
                    #ifdef DEBUG_COLLISIONS
                        titleVars.offset = 3;
                    #endif

                }         
                
                if (PC::buttons.pressed(BTN_LEFT)) { 

                    titleVars.cursor = 0;

                }

                if (PC::buttons.pressed(BTN_RIGHT)) { 

                    titleVars.cursor = 1;

                }

                PD::setColor(Constants::ColourTitleBackground);
                PD::fillRect(74, 8, 44, 15);
                PD::setColor(Constants::ColourBlack);
                PD::drawRect(16 + (titleVars.cursor == 1 ? 30 : 0), 71, 2, 2);

                PD::drawBitmap(22, 70, Images::Title_Start);
                PD::drawBitmap(52, 70, Images::Title_HighScore);

            }

            break;

        case TitleState::Aim:

            PD::setColor(Constants::ColourBlack);
            PD::fillRect(15, 14, 80, 38);
            PD::drawBitmap(13, 12, Images::Corner_TL);
            PD::drawBitmap(13, 46, Images::Corner_BL);
            PD::drawBitmap(94, 12, Images::Corner_TR);
            PD::drawBitmap(94, 46, Images::Corner_BR);

            PD::setColor(Constants::ColourBlack);
            PD::drawFastHLine(16, 12, 78);
            PD::drawFastHLine(16, 53, 78);
            PD::drawFastVLine(13, 20, 32);
            PD::drawFastVLine(96, 20, 32);

            PD::setColor(Constants::ColourWhite);
            PD::drawFastHLine(16, 13, 78);
            PD::drawFastHLine(16, 52, 78);
            PD::drawFastVLine(14, 20, 31);
            PD::drawFastVLine(95, 20, 31);

            PD::drawBitmap(18, 16, Images::Aim);

            if (PC::buttons.pressed(BTN_A)) { 

                if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, REPEAT_VAL)) {

                    this->gamePlayVars.setGameLevel(GameLevel::Easy);
                    this->car.setTransmissionType(TransmissionType::Auto);
                    this->gameState = GameState::Game_Init;

                }
                else {

                    titleVars.titleState = TitleState::SelectTransmission;
                    this->playSoundEffect_FromSD(this->car.getTransmissionType() == TransmissionType::Auto ? SoundEffects::Auto : SoundEffects::Manual, false);

                }

            }  

            break;

        case TitleState::SelectTransmission:

            PD::drawBitmap(18, 16, Images::TransmissionType_Auto);
            PD::drawBitmap(60, 16, Images::TransmissionType_Manual);
            PD::drawBitmap(16 + (this->car.getTransmissionType() == TransmissionType::Manual ? 42 : 0), 14, Images::TransmissionType_Highlight);

            if (PC::buttons.pressed(BTN_A)) { 

                this->gameState = GameState::Game_Init;

            }         
            
            if (PC::buttons.pressed(BTN_LEFT) && this->car.getTransmissionType() == TransmissionType::Manual) { 

                this->playSoundEffect_FromSD(SoundEffects::Auto, false);
                this->car.setTransmissionType(TransmissionType::Auto);
                this->gamePlayVars.setGameLevel(GameLevel::Auto);

            }

            if (PC::buttons.pressed(BTN_RIGHT) && this->car.getTransmissionType() == TransmissionType::Auto) { 

                this->playSoundEffect_FromSD(SoundEffects::Manual, false);
                this->car.setTransmissionType(TransmissionType::Manual);
                this->gamePlayVars.setGameLevel(GameLevel::Manual);

            }

            break;

    }


    houseKeeping();
    moveRoadSide();
    
    this->cameraPos.setZ(this->cameraPos.getZ() + 6);
    this->cameraPos.setY(this->world.roadHeightAt(this->cameraPos.getZ() + 2 * UPM) + UPM);


    // Reset the time to ensure a white (day) screen ..

    this->gamePlayVars.setTime(700);

}
