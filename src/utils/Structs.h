#pragma once

#include <stdint.h>
#include "Constants.h"

struct Collision {

    bool collision;
    bool nudged;
    uint8_t carIndex;
    Direction direction = Direction::None;

    void reset() {

        this->collision = false;
        this->nudged = false;
        this->carIndex = 0;
        Direction direction = Direction::None;
        
    }

};


struct TitleVars {

    uint8_t cursor = 0;
    uint8_t offset = 0;
    bool flyIn = true;
    TitleState titleState = TitleState::StartGame;
    bool seenAim = false;

    void reset() {

        this->offset = 0;
        this->flyIn = true;
        this->titleState = TitleState::StartGame;

    }

};

struct SplashScreenVariables {

    uint8_t counter = 0;
    uint8_t spindle = 0;
    SplashScreenState splashScreenState = SplashScreenState::PPOT;

    void reset() {

        this->counter = 0;
        this->splashScreenState = SplashScreenState::PPOT;

    }

};

struct HighScoreVars {

    uint8_t alternate = 0;
    uint8_t clearScores = 0;    
    uint8_t cursor;

};

struct GameOverVars {

    uint8_t offset = 0;
    uint8_t finalSpeed = 0;

    void reset() {

        this->offset = 0;
        this->finalSpeed = 0;
    }

};