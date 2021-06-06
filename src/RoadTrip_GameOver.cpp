#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::gameOver_Init() {

    this->gameState = GameState::GameOver;
    
}   


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::gameOver() {

    draw(false, true);

    renderPlayerCar();
    renderHud(false);
    renderGameOver();


    if (PC::buttons.pressed(BTN_A)) { 

        this->gameState = GameState::SaveScore;

    }

    houseKeeping();
    moveRoadSide();
    
    this->cameraPos.setZ(this->cameraPos.getZ() + gameOverVars.finalSpeed);
    this->cameraPos.setY(this->world.roadHeightAt(this->cameraPos.getZ() + 2 * UPM) + UPM);

    this->gamePlayVars.setTime(700);

}
