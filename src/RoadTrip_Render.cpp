#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


void Game::renderHud(bool renderTacho) {


    // Gearbox ..

    switch (this->car.getTransmissionType()) {

        case TransmissionType::Manual:
            {
                uint8_t frame = (PC::frameCount % 8) / 2;
                PD::drawBitmap(1, 1, Images::Gearstick);

                switch (this->car.getGear()) {

                    case 0:
                        PD::drawBitmap(7 + ((frame % 3) - 1), 7 + ((frame % 3) - 1), Images::Gearstick_Knob);
                        break;

                    case 1 ... 5:
                        PD::drawBitmap(pgm_read_byte(&Constants::GearboxX[this->car.getGear()]), pgm_read_byte(&Constants::GearboxY[this->car.getGear()]), Images::Gearstick_Knob);
                        break;

                    default: break;

                }

            }
            
            break;

        case TransmissionType::Auto:
        
            PD::drawBitmap(3, 1, Images::AutoD);
            PD::drawBitmap(20, 6, Images::AutoNumbers[this->car.getGear() - 1]);
            break;

    }


    // Render Tacho ..

    if (renderTacho) {

        uint8_t tacho = this->car.getTacho();
        PD::drawBitmap(1, 17 + (this->car.getTransmissionType() == TransmissionType::Manual ? 3 : 0), Images::Tacho[tacho - 1]);

    }


    // Render distance travelled ..

    {

        PD::drawBitmap(36, 1, Images::Odometer);


		uint8_t digits[5] = {};
		extractDigits(digits, static_cast<uint16_t>(this->gamePlayVars.distance / 10));
		
		for (uint8_t i = 0, x2 = 61 ; i < 5; ++i, x2 -= 6) {
			PD::drawBitmap(x2, 2, Images::Font3x5_Black[digits[i]]);
		}

		PD::drawBitmap(67, 2, Images::Font3x5_White[this->gamePlayVars.distance % 10]);

	}



    // Render this->car counter ..

    PD::drawBitmap(86, 1, Images::CarPassPanel);

    if (this->gamePlayVars.carsPassed > 0 && (this->gamePlayVars.getTime() > 3000) && getFrameCount(32) == 0) {

        this->playSoundEffect(SoundEffects::Beep);

    }

    if (this->gamePlayVars.carsPassed == 0) {
		
		for (uint8_t i = 0, x2 = 101; i < 3; ++i, x2 -= 6) {
			PD::drawBitmap(x2, 4, Images::CarPassPanel_Check);
		}

    }
    else if ((this->gamePlayVars.carsPassed > 0 && this->gamePlayVars.getTime() > 3000 && this->getFrameCountHalf(32)) || this->gamePlayVars.getTime() <= 3000) {

		uint8_t digits[3] = {};
		extractDigits(digits, this->gamePlayVars.carsPassed);
		
		for (uint8_t i = 0, x2 = 101; i < 3; ++i, x2 -= 6) {
			PD::drawBitmap(x2, 4, Images::Font3x5_Black[digits[i]]);
		}

	}

}

void Game::renderPlayerCar() {


    // Calculate an index for the wheel graphic using the speed ..

    uint8_t index = 0;
    uint8_t carSpeed = this->car.getSpeed_Display();

    switch (carSpeed) {

        case 0 ... 1:
            index = 1;
            break;

        case 2 ... 14:
            index = 9 - (carSpeed / 2);
            break;

        default:
            index = 2;
            break;

    }

    index = this->getFrameCountHalf(index);

    switch (this->car.getSteeringDirection()) {

        case Direction::Left:
            index = index + 2;
            break;

        case Direction::Right:
            index = index + 4;
            break;

        default: break;
            
    }
  
    
    // Set this->car X to camera X (as this->car is always in the middle of screen) ..

    this->car.setX(this->cameraPos.getX());
    this->car.setY(this->world.roadHeightAt(this->car.getZ()));


    // Render the this->car ..

    Vec3 playerNonRef = this->car.clone();
    Vec3 carPerspective = this->world.perspective(playerNonRef, this->cameraPos);
    carPerspective.setY(carPerspective.getY());


    #ifndef DEBUG_DO_NOT_DRAW_CAR

        PD::drawBitmap(39, 86 - Constants::PlayerCarHeightPX, Images::Player[index + (this->gamePlayVars.getGameLevel() == GameLevel::Easy ? 6 : 0)]);

    #endif

    #ifdef DEBUG_COLLISIONS

        Vec3 leftTopCorner = this->car.clone();
        leftTopCorner.setX(leftTopCorner.getX() - (Constants::PlayerCarWidthUnits / 2));
        leftTopCorner.setZ(leftTopCorner.getZ() + Constants::PlayerCarLengthUnits);

        Vec3 rightTopCorner = this->car.clone();
        rightTopCorner.setX(rightTopCorner.getX() + (Constants::PlayerCarWidthUnits / 2));
        rightTopCorner.setZ(rightTopCorner.getZ() + Constants::PlayerCarLengthUnits);
        
        Vec3 leftBottomCorner = this->car.clone();
        leftBottomCorner.setX(leftBottomCorner.getX() - (Constants::PlayerCarWidthUnits / 2));

        Vec3 rightBottomCorner = this->car.clone();
        rightBottomCorner.setX(rightBottomCorner.getX() + (Constants::PlayerCarWidthUnits / 2));
        
        Vec3 leftTopCornerPersepective = this->world.perspective(leftTopCorner, this->cameraPos);
        Vec3 rightTopCornerPersepective = this->world.perspective(rightTopCorner, this->cameraPos);
        Vec3 leftBottomCornerPersepective = this->world.perspective(leftBottomCorner, this->cameraPos);
        Vec3 rightBottomCornerPersepective = this->world.perspective(rightBottomCorner, this->cameraPos);

        PD::drawLine(leftTopCornerPersepective.getX(), leftTopCornerPersepective.getY(), rightTopCornerPersepective.getX(), rightTopCornerPersepective.getY());
        PD::drawLine(rightTopCornerPersepective.getX(), rightTopCornerPersepective.getY(), rightBottomCornerPersepective.getX(), rightBottomCornerPersepective.getY());
        PD::drawLine(rightBottomCornerPersepective.getX(), rightBottomCornerPersepective.getY(), leftBottomCornerPersepective.getX(), leftBottomCornerPersepective.getY());
        PD::drawLine(leftBottomCornerPersepective.getX(), leftBottomCornerPersepective.getY(), leftTopCornerPersepective.getX(), leftTopCornerPersepective.getY());

    #endif


    // Render brake cloud if braking ..

    if (this->gamePlayVars.cloudCount > 0) {

        this->gamePlayVars.cloudCount--;

        uint8_t idx = (this->gamePlayVars.cloudCount / 2) % 3;

        switch (this->gamePlayVars.cloudType) {

            case CloudType::Normal:

                if (this->gamePlayVars.cloudSize != Direction::Right)     PD::drawBitmap(31, 80, Images::Dirt_Cloud_Left[idx]);
                if (this->gamePlayVars.cloudSize != Direction::Left)      PD::drawBitmap(59, 80, Images::Dirt_Cloud_Right[idx]);
                break;

            case CloudType::Flick:

                if (this->gamePlayVars.cloudSize != Direction::Right)     PD::drawBitmap(31, 79, Images::Flick_Cloud_Left[idx]);
                if (this->gamePlayVars.cloudSize != Direction::Left)      PD::drawBitmap(59, 79, Images::Flick_Cloud_Right[idx]);
                break;

        }

        if (this->gamePlayVars.cloudCount == 0) this->gamePlayVars.cloudType = CloudType::None;

    }

}



void Game::drawFastSolidLine(int16_t startPos, int16_t endPos, int16_t y, uint8_t color) {

    PD::setColor(color);
    PD::drawFastHLine(startPos, y, endPos - startPos + 1);

}

void Game::renderRoadEdge(int16_t &startPos, int16_t endPos, int16_t y, uint8_t color) {

    drawFastSolidLine(startPos, endPos + 1, y, color);
    startPos = endPos;

}


void Game::renderRoad(int16_t &startPos, int16_t endPos, int16_t y, uint8_t color) {

    drawFastSolidLine(startPos, endPos, y, color);
    startPos = endPos;

}

void Game::renderDayBanner() {
    
    uint8_t x = 0;

    if (this->gamePlayVars.showDayBannerCount > 0) {

        PD::drawBitmap(7, 14, Images::Banner);

        switch (this->gamePlayVars.getDay()) {

            case 1 ... 9:
                x = 39 + 6;
                break;

            case 10 ... 99:
                x = 39 + 2;
                break;

            case 100 ... 255:
                x = 39;
                break;

        }

        PD::drawBitmap(x, 24, Images::Day);
        x = x + 18;

        {
            uint8_t newDay = this->gamePlayVars.getDay();
            uint8_t origNewDay = newDay;
            if (newDay >= 100)                      { PD::drawBitmap(x, 24, Images::Font4x6_Orange[newDay / 100]);  newDay = newDay - ((newDay / 100) * 100); x = x + 5; }
            if (newDay >= 10 || origNewDay > 100)   { PD::drawBitmap(x, 24, Images::Font4x6_Orange[newDay / 10]);  newDay = newDay % 10; x = x + 5; }
            PD::drawBitmap(x, 24, Images::Font4x6_Orange[newDay]);

        }

    }

}

void Game::renderGameOver() { 

    PD::drawBitmap(DISPLAY_WIDTH - gameOverVars.offset + 22, 21, Images::GameOver_00);
    PD::drawBitmap(-59 + gameOverVars.offset - 23, 40, Images::GameOver_01);

}

void Game::renderLights() {

    uint8_t x = 0;

    if (this->gamePlayVars.showDayBannerCount > 0) {


        PD::drawBitmap(46, 12, Images::Lights);

        switch (this->gamePlayVars.showDayBannerCount) {

            case 0 ... 19:
                renderLights_Shown(7);
                break;

            case 20:
                renderLights_Shown(7);
                this->playSoundEffect_FromSD(SoundEffects::Go, false);
                break;

            case 21 ... 49:
                renderLights_Shown(7);
                break;

            case 50:
                renderLights_Shown(7);
                this->playSoundEffect_FromSD(SoundEffects::One, false);
                break;

            case 51 ... 79:
                renderLights_Shown(3);
                break;

            case 80:
                renderLights_Shown(3);
                this->playSoundEffect_FromSD(SoundEffects::Two, false);
                break;

            case 81 ... 109:
                renderLights_Shown(1);
                break;

            case 110:
                renderLights_Shown(1);
                this->playSoundEffect_FromSD(SoundEffects::Three, false);
                break;

            case 111 ... 130:
                break;

        }

    }

}

void Game::renderLights_Shown(uint8_t lights) {

    PD::setColor(WHITE);

    if (lights == 7)    PD::drawBitmap(52, 28, Images::Light_Green);
    if (lights == 3)    PD::drawBitmap(52, 22, Images::Light_Yellow);
    if (lights == 1)    PD::drawBitmap(52, 16, Images::Light_Red);

}
