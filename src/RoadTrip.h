#pragma once

#include "Pokitto.h"
#include <LibAudio>
#include <LibSchedule>
#include "entities/Entities.h"
#include "Images/Images.h"
#include "SDFileSystem.h"
#include "Utils/Utils.h"
#include "Utils/Structs.h"
#include "Utils/GamePlayVars.h"
#include "Utils/GameCookie.h"
#include "Utils/SFXVolumeSource.hpp"
#include "sounds/Sounds.h"

using PC=Pokitto::Core;
using PD=Pokitto::Display;
using PB=Pokitto::Buttons;


class Game {

    private:

        GameCookie *cookie;

        Vec3 roadSide[Constants::NumberOfRoadSide];
        Vec3 snowFlakes[Constants::NumberOfSnowFlakes];

        Vec3 cameraPos;
        Car car;
        World world;
        OtherCar otherCars[Constants::NumberOfOtherCars];
        GameState gameState = GameState::Splash_Init;

        SplashScreenVariables splashScreenVariables;
        HighScoreVars highScoreVars;
        GamePlayVars gamePlayVars;
        GameOverVars gameOverVars;
        TitleVars titleVars;

        File mainThemeFile;
        File soundEffectFile;
        
        Audio::RAWFileSource *music;

    public:

        void setup(GameCookie *cookie);
        void loop();

    private:

        // Play
        void game_Init();
        void game();
        void draw(bool drawOtherCars, bool noCurves);

        // Render
        void renderHud(bool renderTacho);
        void renderPlayerCar();
        void drawFastSolidLine(int16_t startPos, int16_t endPos, int16_t y, uint8_t color);
        void renderRoadEdge(int16_t &startPos, int16_t endPos, int16_t y, uint8_t color);
        void renderRoad(int16_t &startPos, int16_t endPos, int16_t y, uint8_t color);
        void renderDayBanner();
        void renderGameOver();
        void renderLights();
        void renderLights_Shown(uint8_t lights);

        // Game Over
        void gameOver_Init();
        void gameOver();

        // Splash Screen
        void splashScreen_Init();
        void splashScreen();

        // Title
        void title_Init();
        void title();

        // Radio
        void radio_Init();
        void radio();

        // Utils
        void swap(OtherCar *xp, OtherCar *yp);  
        void sort(OtherCar arr[], uint8_t n);  
        void moveCar(int16_t xMovement, int16_t zMovement, Collision &collision);
        void moveOtherCars();
        void houseKeeping();
        bool nudgeCarForward(uint8_t carIdx);
        uint8_t getFrameCount() const;
        uint8_t getFrameCount(uint8_t mod) const;

        bool getFrameCountHalf(uint8_t mod) const;
        bool isFrameCount(uint8_t mod) const;
        bool isFrameCount(uint8_t mod, uint8_t val) const;
        void displayClearToPattern(uint8_t idx);
        void playTheme(uint8_t themeToPlay, uint8_t &currentTheme, bool mute, bool updateTheme);
        void playSoundEffect_FromSD(SoundEffects soundEffect, bool doNotCut);
        void playSoundEffect(SoundEffects soundEffect);

        void moveRoadSide();
        void placeRoadSide();

        // High Score
        void HighScore_Init();
        void renderHighScore();

};

