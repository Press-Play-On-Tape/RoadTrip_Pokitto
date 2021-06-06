#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

void Game::swap(OtherCar *xp, OtherCar *yp) {  

    OtherCar temp = *xp;  
    *xp = *yp;  
    *yp = temp;  

}  
  
void Game::sort(OtherCar arr[], uint8_t n) {  

    uint8_t i, j;  

    for (i = 0; i < n-1; i++) {     

        for (j = 0; j < n-i-1; j++) {

            if (arr[j].getZ() < arr[j+1].getZ()) {  
                swap(&arr[j], &arr[j+1]);  
            }

        }

    }

}  

void Game::moveCar(int16_t xMovement, int16_t zMovement, Collision &collision) { // will return NoCollision constant or this->car index.

    collision.reset();

    #ifdef DEBUG_COLLISIONS

        printf("moveCars(%i, %i)\n", xMovement, zMovement);

    #endif

    uint8_t collide = Constants::NoCollision;

    for (uint8_t i = 0; i < this->gamePlayVars.numberOfOtherCars; i++) {

        OtherCar &otherCar = otherCars[i];

        #ifdef DEBUG_COLLISIONS

            printf("A P ");
            this->car.printPos();
            printf(" + X(%i) + x(%i) ", xMovement, zMovement);
            printf(" = O(%i) ", i);
            otherCar.printPos();
            printf(" - ");

        #endif

        int16_t xOverlap = (this->car.getX() + xMovement) - (otherCar.getX() + otherCar.getXWorld());
        int16_t zOverlap = (this->car.getZ() + zMovement) - otherCar.getZ();

        if (abs(xOverlap) < Constants::OtherCarWidthUnits && abs(zOverlap) < Constants::OtherCarLengthUnits / 2) {
                    
            #ifdef DEBUG_COLLISIONS
                printf("COL x: %i, xO: %i, zO: %i\n", xMovement, xOverlap, zOverlap);
            #endif

            if (xMovement != 0) { // Move left or right ..

                if (abs(zOverlap) < Constants::CollisionTollerance_Z) {

                    collision.collision = true;
                    collision.carIndex = i;
                    collision.direction = Direction::LeftRight;

                    this->playSoundEffect(SoundEffects::Bump);

                    #ifdef DEBUG_COLLISIONS
                        printf("BLOCK LR\n");
                    #endif

                    break;

                }

            }

            if (zMovement != 0) { // Move front or back ..

                if (abs(xOverlap) < Constants::CollisionTollerance_X) {

                    collision.collision = true;
                    collision.carIndex = i;
                    collision.direction = Direction::FrontBack;

                    this->playSoundEffect(SoundEffects::Bump);

                    #ifdef DEBUG_COLLISIONS
                        printf("BLOCK FB\n");
                    #endif

                    if (this->car.getZ() < otherCars[i].getZ() && nudgeCarForward(i)) {

                        collision.nudged = true;

                    }

                    break;
                    
                }

            }

            #ifdef DEBUG_COLLISIONS
                printf("No Col\n");
            #endif

        }
        else {
            
            #ifdef DEBUG_COLLISIONS
                printf("\n");
            #endif

        }

    }

}

void Game::moveOtherCars() {

    // Sort the cars ..

    sort(otherCars, this->gamePlayVars.numberOfOtherCars);

    for (uint8_t i = 0; i < this->gamePlayVars.numberOfOtherCars; i++) {

        bool collide = false;
        OtherCar &otherCar = otherCars[i];

        int16_t otherCarX = otherCar.getX() + otherCar.getXWorld() + otherCar.getXMovement();


        #ifdef DEBUG_COLLISIONS

            printf("C O(%i) ", i);
            otherCar.printPos();
            printf(" - "); 

        #endif        

        for (uint8_t j = i; j < this->gamePlayVars.numberOfOtherCars; j++) {

            if (i != j) {

                OtherCar &testCar = otherCars[j];

                if (abs((testCar.getX() + testCar.getXWorld()) - otherCarX) < Constants::OtherCarWidthUnits && abs(testCar.getZ() - (otherCar.getZ() + otherCar.getSpeed_Display())) < Constants::OtherCarLengthUnits ) {

                    collide = true;

                }

            }

        }


        // What about into the player?

        if (abs(this->car.getX() - otherCarX) < Constants::OtherCarWidthUnits && abs((this->car.getZ() + this->car.getSpeed_Display()) - (otherCar.getZ() + otherCar.getSpeed_Display())) < 8 ) {

            collide = true;

        }


        // If no collision has occurred then move the this->car forward ..

        if (!collide) {

            otherCar.setZ(otherCar.getZ() + otherCar.getSpeed_Display());

            if (otherCar.getZ() < this->cameraPos.getZ() + Constants::DistToRenewCars) {

                if (this->gamePlayVars.numberOfOtherCars > 1) {

                    int16_t dist = otherCars[0].getZ() - this->cameraPos.getZ();

                    if (dist < 700) {

                        otherCar.setZ(otherCars[0].getZ() + random(700 - dist, 1200 - dist));

                    }
                    else {

                        otherCar.setZ(this->cameraPos.getZ() + random(700, 1200));

                    }

                }
                else {

                    otherCar.setZ(this->cameraPos.getZ() + random(700, 1200));

                }


                // Position X this->cars ..

                if (this->gamePlayVars.getGameLevel() == GameLevel::Easy) {

                    switch (random(0, 3)) {

                        case 0:
                            otherCar.setX(-74);
                            break;

                        case 1:
                            otherCar.setX(0);
                            break;

                        case 2:
                            otherCar.setX(74);
                            break;


                    }
                } 

                otherCar.setSpeed(random(9, 14) * Constants::SpeedDiv);

                if (this->gamePlayVars.carsPassed > 0 && !this->gamePlayVars.gameOver) {

                    this->gamePlayVars.carsPassed--;

                    if (this->gamePlayVars.carsPassed == 0) {

                        this->playSoundEffect_FromSD(SoundEffects::Perfect, true);

                    }
                    else {
                
                        this->playSoundEffect(SoundEffects::Passing);

                    }

                }
                else {
                                    
                    this->playSoundEffect(SoundEffects::Passing);

                }

            }

            otherCar.update(this->gamePlayVars.getGameLevel());

        }

    }

    // Has the game finished?

    if (this->gamePlayVars.gameOver) {

        if (gameOverVars.offset < Constants::GameOver_MaxOffset) {

            gameOverVars.offset = gameOverVars.offset + 3;
            renderGameOver();

        }
        else {

            renderGameOver();
            gameOverVars.finalSpeed = this->car.getSpeed_Display();
            this->gameState = GameState::GameOver_Init;

        }

    }

}

void Game::houseKeeping() {

    if (this->cameraPos.getZ() > Constants::ZMax) {

        this->cameraPos.decZ();
        this->car.decZ();
        this->world.decZ();
        this->gamePlayVars.decZ();

        for (uint8_t i = 0; i < this->gamePlayVars.numberOfOtherCars; i++) {

            OtherCar &otherCar = otherCars[i];
            otherCar.decZ();

        }

        for (uint8_t i = 0; i < Constants::NumberOfRoadSide; i++) {

            Vec3 &roadsidePos = roadSide[i];
            roadsidePos.decZ();

        }

    }
    else {

        // Are this->cars too far into the distance?

        for (uint8_t i = 0; i < this->gamePlayVars.numberOfOtherCars; i++) {

            OtherCar &otherCar = otherCars[i];
            if (otherCar.getZ() - otherCar.getZ() > 2500) {
                otherCar.setZ(otherCar.getZ() - 1500);
            }

        }

    }


    // Increase the odometer ..

    if (!this->gamePlayVars.gameOver && (this->cameraPos.getZ() - this->gamePlayVars.previousZ > 64)) {

        this->gamePlayVars.previousZ = this->cameraPos.getZ();
        this->gamePlayVars.distance++;

    }


    // Increase time of day ..

    bool newDay = this->gamePlayVars.incTimeOfDay();

    if (newDay) {

        if (this->gamePlayVars.carsPassed > 0) {

            this->gamePlayVars.gameOver = true;
            this->playSoundEffect_FromSD(SoundEffects::GameOver, false);

        }
        else {

            this->gamePlayVars.showDayBannerCount = Constants::NewDayBannerDelay;
            this->gamePlayVars.carsPassed = (this->gamePlayVars.getGameLevel() == GameLevel::Easy ? Constants::CarsToPass_Easy_Init : Constants::CarsToPass_Normal_Init) + (this->gamePlayVars.getDay() * (this->gamePlayVars.getGameLevel() == GameLevel::Easy ? Constants::CarsToPass_Easy_Increase : Constants::CarsToPass_Normal_Increase));
            this->playSoundEffect_FromSD(SoundEffects::TimeExtension, true);

        }
        
    }


    // Decrease banner counter ..

    this->gamePlayVars.decShowDayBannerCount();

}

void Game::moveRoadSide() {

    // Move roadSide

    for (uint8_t i = 0; i < Constants::NumberOfRoadSide; i++) {

        Vec3 &roadSidePos = roadSide[i];

        if (roadSidePos.getZ() < this->cameraPos.getZ() - Constants::DistToRenewCars) {
            roadSidePos.setZ(this->cameraPos.getZ() + random(600, 900));
            roadSidePos.setX(random(0,2));
        }

    }

}

void Game::placeRoadSide() {

    for (uint8_t i = 1; i <= Constants::NumberOfRoadSide; i++) {

        Vec3 &roadSidePos = roadSide[i - 1];

        roadSidePos.setX(i % 2);
        roadSidePos.setZ(400 + (i * 245));

    }

}

bool Game::nudgeCarForward(uint8_t carIdx) {

    OtherCar &otherCar = otherCars[carIdx];
    bool collide = false;

    int16_t otherCarZ = otherCar.getZ() + otherCar.getSpeed_Display() + Constants::NudgeDistance;
    int16_t otherCarX = otherCar.getX() + otherCar.getXWorld() + otherCar.getXMovement();

    for (uint8_t i = 0; i < this->gamePlayVars.numberOfOtherCars; i++) {

        if (i != carIdx) {

            OtherCar &testCar = otherCars[i];

            if (abs((testCar.getX() + testCar.getXWorld()) - otherCarX) < Constants::OtherCarWidthUnits && abs(testCar.getZ() - otherCarZ) < Constants::OtherCarLengthUnits ) {

                collide = true;

            }

        }

    }

    if (!collide) {

        otherCar.setZ(otherCar.getZ() + Constants::NudgeDistance);
        return true;

    }

    return false;

}


uint8_t Game::getFrameCount() const {

    return PC::frameCount;

}

uint8_t Game::getFrameCount(uint8_t mod) const {

    return PC::frameCount % mod;

}

bool Game::getFrameCountHalf(uint8_t mod) const {

	return this->getFrameCount(mod) >= (mod / 2);

}

bool Game::isFrameCount(uint8_t mod) const {

    return (PC::frameCount % mod) == 0;

}

bool Game::isFrameCount(uint8_t mod, uint8_t val) const {

    return (PC::frameCount % mod) == val;

}

void Game::displayClearToPattern(uint8_t idx) {

    const uint8_t fill[] = { 56, 57, 58, 59, 60, 61, 62, };
    PD::fillScreen(fill[idx]);

}


void Game::playTheme(uint8_t themeToPlay, uint8_t &currentTheme, bool mute, bool updateTheme) {

    constexpr char sounds[5][19] = { "music/roadtr03.raw", "music/roadtr04.raw", 
                                     "music/roadtr05.raw", "music/roadtr01.raw", 
                                     "music/roadtr02.raw",  };

    if (!mute) {

        if (!updateTheme || currentTheme != themeToPlay) {

            if (mainThemeFile.openRO(sounds[themeToPlay])) {
                auto& music = Audio::play<0>(mainThemeFile);
                music.setLoop(true);
            }

            if (updateTheme) currentTheme = themeToPlay;

        }

    }
    else {

        Audio::stop<0>();

    }

}

void Game::playSoundEffect_FromSD(SoundEffects soundEffect, bool doNotCut) {

    if (doNotCut && !music->ended()) { return; }

    constexpr char sounds[13][19] = { "music/roadtr20.raw", "music/roadtr21.raw",
                                      "music/roadtr22.raw", "music/roadtr23.raw",
                                      "music/roadtr24.raw", "music/roadtr25.raw", 
                                      "music/roadtr26.raw", "music/roadtr27.raw", 
                                      "music/roadtr28.raw", "music/roadtr29.raw",
                                      "music/roadtr30.raw", "music/roadtr31.raw",
                                      "music/roadtr32.raw",
                                    };

    if (soundEffectFile.openRO(sounds[static_cast<uint8_t>(soundEffect)])) {
        this->music = &Audio::play<2>(soundEffectFile);
        this->music->setLoop(false);
    }

}

void Game::playSoundEffect(SoundEffects soundEffect) {

    #ifdef SOUNDS
        
        uint8_t vol = 255;

        if (!music->ended()) { return; }

        switch (soundEffect) {
            
            case SoundEffects::Bump:
                Audio::play<1>(Sounds::sfx_Bump, 128, 2);        
                break;
            
            case SoundEffects::Passing:
                Audio::play<1>(Sounds::sfx_Passing, 255, 2);        
                break;
            
            case SoundEffects::ButtonPress:
                Audio::play<1>(Sounds::sfx_ButtonPress, 255, 2);        
                break;
                        
            case SoundEffects::Beep:
                Audio::play<1>(Sounds::sfx_ButtonPress, 255, 2);        
                break;
                        
            case SoundEffects::Skid:
                Audio::play<1>(Sounds::sfx_Skid, 255, 2);        
                break;
                    
        }

    #endif

}    