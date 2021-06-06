#pragma once

#include "Pokitto.h"
#include "PokittoCookie.h"
#include "Constants.h"

struct GamePlayVars {
    
    uint16_t distance = 0;
    uint16_t oldIndexTo = 0;
    uint16_t previousZ = 0;
    uint8_t showDayBannerCount = Constants::NewDayBannerDelay_CountDown;
    uint8_t days = 1;
    uint16_t timeOfDay = 100;
    uint16_t horizonX = 64;
    uint8_t numberOfOtherCars = 0;
    uint8_t carsPassed = 0;
    uint8_t radioButtonCount = 0;
    uint8_t radioOffset = 50;
    uint8_t theme = 50;

    bool gameOver = false;
    bool newRecord = false;

    Direction radioDirection = Direction::None;
    RadioButton radioButton = RadioButton::None;
    GameLevel gameLevel = GameLevel::Auto;


    // Dirt cloud ..

    uint8_t cloudCount = 0;
    Direction cloudSize = Direction::None;
    CloudType cloudType = CloudType::None;


    uint8_t getDay()                    { return days; }
    uint16_t getTime()                  { return timeOfDay; }
    GameLevel getGameLevel()            { return gameLevel; }
    bool getNewRecord()                 { return newRecord; }

    void setDay(uint8_t val)            { this->days = val; }
    void setTime(uint16_t val)          { this->timeOfDay = val; }
    void setGameLevel(GameLevel val)    { this->gameLevel = val; }
    void setNewRecord(bool val)         { this->newRecord = val; }

    void decZ() {

        this->previousZ = this->previousZ - Constants::ZReduction;

    }

    void reset() {

        this->distance = 0;
        this->carsPassed = 0;
        this->oldIndexTo = 0;
        this->previousZ = 0;
        this->showDayBannerCount = Constants::NewDayBannerDelay_CountDown;
        this->days = 1;
        this->cloudCount = 0;
        this->cloudSize = Direction::None;
        this->timeOfDay = 100;
        this->gameOver = false;
        this->numberOfOtherCars = (Constants::NumberOfOtherCars > 2 ? 2 : Constants::NumberOfOtherCars);
        this->newRecord = false;

        switch (this->getGameLevel()) {

            case GameLevel::Easy:
                this->carsPassed = Constants::CarsToPass_Easy_Init;
                break;

            case GameLevel::Auto:
            case GameLevel::Manual:
                this->carsPassed = Constants::CarsToPass_Normal_Init;
                break;

        }

    }

    TimeOfDay getTimeOfDay() {

        switch (this->timeOfDay) {

            case 0 ... 1700:
                return TimeOfDay::Day;

            case 1701 ... 2100:
                return TimeOfDay::Dawn;

            case 2101 ... 2800:
                return TimeOfDay::Night;

            case 2801 ... 3200:
                return TimeOfDay::Dawn;

            default:
                return TimeOfDay::Day;

        }

        return TimeOfDay::Day;

    }


    uint8_t getTimeOfDay_Pattern() {

        switch (this->timeOfDay) {

            case    1 ... 1400:     return Constants::Sky_BX______D;
            case 1401 ... 1500:     return Constants::Sky_B_X_____D;
            case 1501 ... 1600:     return Constants::Sky_B__X____D;
            case 1601 ... 1700:     return Constants::Sky_B___X___D;
            case 1701 ... 1800:     return Constants::Sky_B____X__D;
            case 1801 ... 1900:     return Constants::Sky_B_____X_D;
            case 1901 ... 2700:     return Constants::Sky_B______XD;
            case 2701 ... 2800:     return Constants::Sky_B_____X_D;
            case 2801 ... 2900:     return Constants::Sky_B____X__D;
            case 2901 ... 3000:     return Constants::Sky_B___X___D;
            case 3001 ... 3100:     return Constants::Sky_B__X____D;
            case 3101 ... 3200:     return Constants::Sky_B_X_____D;
            default:                return Constants::Sky_BX______D;

        }

        return Constants::Sky_BX______D;

    }


    uint8_t getTimeOfDay_PatternForRoad() {

        switch (this->getScenery()) {

            case Scenery::CornField:

                switch (this->timeOfDay) {

                    case    1 ... 1400:     return Constants::Sky_BX______D;
                    case 1401 ... 1600:     return Constants::Sky_B_X_____D;
                    case 1601 ... 1800:     return Constants::Sky_B__X____D;
                    case 1801 ... 2800:     return Constants::Sky_B___X___D;
                    case 2801 ... 3000:     return Constants::Sky_B__X____D;
                    case 3001 ... 3200:     return Constants::Sky_B_X_____D;
                    default:                return Constants::Sky_BX______D;

                }

                return Constants::Sky_BX______D;
                break;

            default:

                switch (this->timeOfDay) {

                    case    1 ... 1400:     return Constants::Sky_BX______D;
                    case 1401 ... 1500:     return Constants::Sky_B_X_____D;
                    case 1501 ... 1600:     return Constants::Sky_B__X____D;
                    case 1601 ... 1700:     return Constants::Sky_B___X___D;
                    case 1701 ... 1800:     return Constants::Sky_B____X__D;
                    case 1801 ... 1900:     return Constants::Sky_B_____X_D;
                    case 1901 ... 2700:     return Constants::Sky_B______XD;
                    case 2701 ... 2800:     return Constants::Sky_B_____X_D;
                    case 2801 ... 2900:     return Constants::Sky_B____X__D;
                    case 2901 ... 3000:     return Constants::Sky_B___X___D;
                    case 3001 ... 3100:     return Constants::Sky_B__X____D;
                    case 3101 ... 3200:     return Constants::Sky_B_X_____D;
                    default:                return Constants::Sky_BX______D;

                }

                return Constants::Sky_BX______D;
                break;

        }

        return Constants::Sky_BX______D;

    }

    bool incTimeOfDay() {

        this->timeOfDay++;

        if (this->timeOfDay > 3200) {
            this->timeOfDay = 0;
        }

        if (this->timeOfDay == 40) {
            
            this->days++;

            switch (this->getGameLevel()) {

                case GameLevel::Auto:
                case GameLevel::Manual:

                    if (this->days == 5) {

                        this->numberOfOtherCars++;

                    }

                    if (this->days == 10) {

                        this->numberOfOtherCars++;

                    }

                    break;

                case GameLevel::Easy:

                    if (this->days == 7) {

                        this->numberOfOtherCars++;

                    }

                    if (this->days == 11) {

                        this->numberOfOtherCars++;

                    }

                    break;


            }

            return true;

        }

        return false;

    }

    void decShowDayBannerCount() {

        if (this->showDayBannerCount > 0) {

            this->showDayBannerCount--;

        }

    }

    Scenery getScenery() {

        switch (this->gameLevel) {

            case GameLevel::Auto:
            case GameLevel::Manual:

                switch (this->days % 10) {
                        
                    case 0:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::CornField;

                            default:
                                return Scenery::Desert;

                        }

                        return Scenery::Desert;

                    case 1:

                        return Scenery::Desert;

                    case 2:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Desert;

                            default:
                                return Scenery::Tropical;

                        }

                        return Scenery::Tropical;

                    case 3:

                        return Scenery::Tropical;

                    case 4:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Tropical;

                            default:
                                return Scenery::Snow;

                        }

                        return Scenery::Snow;

                    case 5:

                        return Scenery::Snow;

                    case 6:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Snow;

                            default:
                                return Scenery::Lights;

                        }

                        return Scenery::Lights;

                    case 7:

                        return Scenery::Lights;

                    case 8:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Lights;

                            default:
                                return Scenery::CornField;

                        }

                        return Scenery::CornField;

                    case 9:

                        return Scenery::CornField;
                }

                return Scenery::Lights;

            case GameLevel::Easy:

                switch (this->days % 5) {
                        
                    case 0:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::CornField;

                            default:
                                return Scenery::Desert;

                        }

                        return Scenery::Desert;

                    case 1:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Desert;

                            default:
                                return Scenery::Tropical;

                        }

                        return Scenery::Tropical;

                    case 2:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Tropical;

                            default:
                                return Scenery::Snow;

                        }

                        return Scenery::Snow;

                    case 3:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Snow;

                            default:
                                return Scenery::Lights;

                        }

                        return Scenery::Lights;

                    case 4:

                        switch (this->timeOfDay) {

                            case 41 ... 2300:
                                return Scenery::Lights;

                            default:
                                return Scenery::CornField;

                        }

                        return Scenery::CornField;


                }

                return Scenery::Lights;


            default: return Scenery::Lights;

        }

    }

};