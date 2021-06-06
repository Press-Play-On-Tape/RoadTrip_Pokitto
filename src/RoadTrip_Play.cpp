#include "RoadTrip.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void Game::game_Init() {

    this->world.reset();
    this->car.reset();
    this->gamePlayVars.reset();
    this->gameOverVars.reset();

    this->cameraPos.setX(0);
    this->cameraPos.setY(UPM);
    this->cameraPos.setZ(0);

    for (uint8_t i = 0; i < this->gamePlayVars.numberOfOtherCars; i++) {

        OtherCar &otherCar = otherCars[i];

        otherCar.setX(0);
        otherCar.setZ(200 + (i * 140));
        otherCar.setSpeed(random(7, 10) * Constants::SpeedDiv);

    }

    placeRoadSide();

    this->gameState = GameState::Game;

    #ifdef DEBUG_COLLISIONS
        this->gamePlayVars.showDayBannerCount = 1;
    #endif


    // Snowflakes ..

    for (uint8_t x = 0; x < Constants::NumberOfSnowFlakes; x++) {

        Vec3 &snowFlake = this->snowFlakes[x];

        snowFlake.setX(random(-2, 108));
        snowFlake.setY(random(-40, -20));
        snowFlake.setZ(random(2, 7));

    }

    this->playTheme(this->cookie->theme, this->gamePlayVars.theme, this->cookie->mute, false);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void Game::game() {


    // Play voices ..

    if (this->gamePlayVars.getTime() == 2800 && this->gamePlayVars.carsPassed > 0) {
        this->playSoundEffect_FromSD(SoundEffects::HurryUp, true);
    }


    if (!this->gamePlayVars.getNewRecord() && abs(this->cookie->getHighScore(this->gamePlayVars.getGameLevel()) - this->gamePlayVars.distance) < 2) {
        this->playSoundEffect_FromSD(SoundEffects::NewRecord, true);
        this->gamePlayVars.setNewRecord(true);
    }

    switch (this->car.getTransmissionType()) {
        
        case TransmissionType::Manual:

            if (PC::buttons.pressed(BTN_UP)) {

                if (this->gamePlayVars.showDayBannerCount <= 20 || this->gamePlayVars.getDay() > 1) {
                    this->car.incGear();
                }
                else {

                    if (this->gamePlayVars.cloudCount == 0 || this->gamePlayVars.cloudType == CloudType::Flick) {

                        this->gamePlayVars.cloudCount = Constants::BrakeCloud_DownGear;
                        this->gamePlayVars.cloudSize = Direction::Both;
                        this->gamePlayVars.cloudType = CloudType::Normal;

                    }

                }

            }

            if (PC::buttons.pressed(BTN_DOWN)) {

                bool brakes = this->car.decGear();

                if (brakes) {

                    this->gamePlayVars.cloudCount = Constants::BrakeCloud_DownGear;
                    this->gamePlayVars.cloudSize = Direction::Both;
                    this->gamePlayVars.cloudType = CloudType::Normal;
                    
                }

            }

            break;

        case TransmissionType::Auto:
        
            if (this->car.getTacho() == 1 && this->car.getGear() > 1) {

                this->car.decGear();

            }
            
            if (!this->car.getOffroad()) {

                if (this->car.getTacho() == 8 && this->car.getGear() > 0) {

                    if (this->gamePlayVars.showDayBannerCount <= 20 || this->gamePlayVars.getDay() > 1) {
                        
                        this->car.incGear();

                        if (this->gamePlayVars.showDayBannerCount == 20 && this->gamePlayVars.getDay() == 1 && this->gamePlayVars.cloudCount > 0) {
                            this->gamePlayVars.cloudCount = this->gamePlayVars.cloudCount + 18;
                        }

                    }
                    else {

                        if (this->gamePlayVars.cloudCount == 0 || this->gamePlayVars.cloudType == CloudType::Flick) {

                            this->gamePlayVars.cloudCount = Constants::BrakeCloud_DownGear;
                            this->gamePlayVars.cloudSize = Direction::Both;
                            this->gamePlayVars.cloudType = CloudType::Normal;

                        }

                    }

                }

            }

            break;

    }


    // Render screen ..

    draw(true, false);
    renderPlayerCar();
    renderHud(true);

    if (this->gamePlayVars.getDay() > 1) {
        renderDayBanner();
    }
    else {
       renderLights();
    }
    

    // Do some house keeping ..

    houseKeeping();
    moveRoadSide();


    // Sort the snow flakes out ..

    for (uint8_t x = 0; x < Constants::NumberOfSnowFlakes; x++) {

        Vec3 &snowFlake = this->snowFlakes[x];

        snowFlake.setY(snowFlake.getY() + snowFlake.getZ());

        if (snowFlake.getY() > 176) {

            snowFlake.setX(random(-2, 108));
            snowFlake.setY(random(-40, -20));
            snowFlake.setZ(random(2, 7));
            
        }

    }
    

    // When moving left or right, the speed must be > 0 .. 
 
    uint8_t speed = this->car.getSpeed_Display();


    // This prevents the this->car from moving on the start line ..

    if (this->gamePlayVars.showDayBannerCount > 20 && this->gamePlayVars.getDay() == 1) {

        speed = 0;

    }



    // Radio

    if (PC::buttons.pressed(BTN_C)) {

        this->gameState = GameState::Radio_Init;
        return;

    }


    // Steering ..
    
    uint8_t speedForSteering = Constants::SpeedSteering[abs( this->car.getSteeringCounter())];
    RoadSegment segClosest = this->world.getRoadSegment(0, this->car.getZ());
    RoadSegment segNext = this->world.getRoadSegment(this->car.getZ(), this->car.getZ() + UPM);
    this->car.setSteeringDirection(Direction::None);
    int16_t carOffsetOnRoad = this->car.getX() - ((segClosest.getX() + segNext.getX()) / 2);


    bool turnLeft = PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, REPEAT_VAL);
    bool turnRight = PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, REPEAT_VAL);

    if ((turnLeft ||  this->car.getSteeringCounter() < 0) && carOffsetOnRoad > -300 && !turnRight) {

        #ifdef DEBUG_COLLISIONS
            printf("Left - ");
        #endif


        // We were turning right previously, so snap back to the counter back to zero to accelerate the left turn ..

        if ( this->car.getSteeringCounter() > 0) {
            this->car.setSteeringCounter(0);
        }


        // Avoid increasing the left turn amount if the player hasn't actually pressed left - ie. the player is not
        // pressing left or right and we are just coasting back to a straight ahead stance ..

        else if (turnLeft) { 

            if ( this->car.getSteeringCounter() > -speed) {
                this->car.setSteeringCounter(this->car.getSteeringCounter() - 1);
            }            
            else if ( this->car.getSteeringCounter() < -speed) {
                this->car.setSteeringCounter(-speed);
            }


            // Are we skidding?

            if (this->car.getSteeringCounter() >= -speed * 0.9 && speed > 20) {

                if (this->gamePlayVars.cloudCount == 0) {

                    this->gamePlayVars.cloudCount = Constants::BrakeCloud_Turn;
                    this->gamePlayVars.cloudSize = Direction::Right; 
                    this->gamePlayVars.cloudType = CloudType::Flick;
                    this->playSoundEffect(SoundEffects::Skid);

                }

            }

        }
        

        Collision collision;

        moveCar(-speedForSteering, 0, collision);

        if (!collision.collision) {
            
            this->cameraPos.setX(this->cameraPos.getX() - speedForSteering);
            this->car.setX(this->car.getX() - speedForSteering);
            this->car.setSteeringDirection(Direction::Left);

            if (segClosest.getSegmentType() == RoadSegmentType::Tunnel) {

                int16_t newCarOffsetOnRoad = this->car.getX() - ((segClosest.getX() + segNext.getX()) / 2);


                // Inside the tunnel and hitting left hand wall ..

                if (newCarOffsetOnRoad > -Constants::OffRoad_Column_Edge && newCarOffsetOnRoad < -Constants::OffRoad_OneWheel) {

                    this->cameraPos.setX(this->cameraPos.getX() - (newCarOffsetOnRoad + Constants::OffRoad_OneWheel));
                    this->car.setX(this->car.getX() - (newCarOffsetOnRoad + Constants::OffRoad_OneWheel));
                    this->car.setSteeringDirection(Direction::None);

                }


                // Outside the tunnel and hitting right hand wall ..

                if (newCarOffsetOnRoad < Constants::OffRoad_Column_Edge && newCarOffsetOnRoad > Constants::OffRoad_OneWheel + 20) {

                    this->cameraPos.setX(this->cameraPos.getX() + (Constants::OffRoad_Column_Edge - newCarOffsetOnRoad));
                    this->car.setX(this->car.getX() + (Constants::OffRoad_Column_Edge - newCarOffsetOnRoad));
                    this->car.setSteeringDirection(Direction::None);

                }

            }

        }

    }

    if ((turnRight ||  this->car.getSteeringCounter() > 0) && carOffsetOnRoad < 300 && !turnLeft) {

        #ifdef DEBUG_COLLISIONS
            printf("Right -");
        #endif


        // We were turning left previously, so snap back to the counter back to zero to accelerate the right turn ..

        if ( this->car.getSteeringCounter() < 0) {
            this->car.setSteeringCounter(0);
        }


        // Avoid increasing the right turn amount if the player hasn't actually pressed right - ie. the player is not
        // pressing left or right and we are just coasting back to a straight ahead stance ..

        else if (turnRight) {

            if ( this->car.getSteeringCounter() < speed) {
                this->car.setSteeringCounter(this->car.getSteeringCounter() + 1);
            }
            else if ( this->car.getSteeringCounter() > speed) {
                this->car.setSteeringCounter(speed);
            }



            // Are we skidding?

            if (this->car.getSteeringCounter() >= speed * 0.9 && speed > 20) {

                if (this->gamePlayVars.cloudCount == 0) {

                    this->gamePlayVars.cloudCount = Constants::BrakeCloud_Turn;
                    this->gamePlayVars.cloudSize = Direction::Left; 
                    this->gamePlayVars.cloudType = CloudType::Flick;
                    this->playSoundEffect(SoundEffects::Skid);

                }

            }

        }


        Collision collision;

        moveCar(speedForSteering, 0, collision);

        if (!collision.collision) {
            this->cameraPos.setX(this->cameraPos.getX() + speedForSteering);
            this->car.setX(this->car.getX() + speedForSteering);
            this->car.setSteeringDirection(Direction::Right);

            if (segClosest.getSegmentType() == RoadSegmentType::Tunnel) {

                int16_t newCarOffsetOnRoad = this->car.getX() - ((segClosest.getX() + segNext.getX()) / 2);

                // Inside the tunnel and hitting right hand wall ..

                if (newCarOffsetOnRoad < Constants::OffRoad_Column_Edge  && newCarOffsetOnRoad > Constants::OffRoad_OneWheel) {

                    this->cameraPos.setX(this->cameraPos.getX() - (newCarOffsetOnRoad - Constants::OffRoad_OneWheel));
                    this->car.setX(this->car.getX() - (newCarOffsetOnRoad - Constants::OffRoad_OneWheel));
                    this->car.setSteeringDirection(Direction::None);

                }
                
                
                // Outside the tunnel and hitting left hand wall ..

                if (newCarOffsetOnRoad > -Constants::OffRoad_Column_Edge && newCarOffsetOnRoad < -Constants::OffRoad_OneWheel - 20) {

                    this->cameraPos.setX(this->cameraPos.getX() - (newCarOffsetOnRoad + Constants::OffRoad_Column_Edge));
                    this->car.setX(this->car.getX() - (newCarOffsetOnRoad + Constants::OffRoad_Column_Edge));
                    this->car.setSteeringDirection(Direction::None);

                }

            }

        }

    }


    // No steering happening? Revert steering back to centre ..

    if (!turnLeft && !turnRight) {

        // switch (this->gamePlayVars.getScenery()) {

        //     case Scenery::Snow:

        //         switch ( this->car.getSteeringCounter()) {

        //             case -100 ... -6:
        //                 this->car.setSteeringCounter(this->car.getSteeringCounter() + 2);
        //                 break;

        //             case -5 ... -1:
        //                 this->car.setSteeringCounter(this->car.getSteeringCounter() + 1);
        //                 break;

        //             case 1 ... 5:
        //                 this->car.setSteeringCounter(this->car.getSteeringCounter() - 1);
        //                 break;

        //             case 6 ... 100:
        //                 this->car.setSteeringCounter(this->car.getSteeringCounter() - 2);
        //                 break;

        //         }

        //         break;

        //     default:

                switch ( this->car.getSteeringCounter()) {

                    case -100 ... -5:
                        this->car.setSteeringCounter(this->car.getSteeringCounter() + 4);
                        break;

                    case -4 ... -3:
                        this->car.setSteeringCounter(this->car.getSteeringCounter() + 2);
                        break;

                    case -2 ... -1:
                        this->car.setSteeringCounter(this->car.getSteeringCounter() + 1);
                        break;

                    case 1 ... 2:
                        this->car.setSteeringCounter(this->car.getSteeringCounter() - 1);
                        break;

                    case 3 ... 4:
                        this->car.setSteeringCounter(this->car.getSteeringCounter() - 2);
                        break;

                    case 5 ... 100:
                        this->car.setSteeringCounter(this->car.getSteeringCounter() - 4);
                        break;

                }

                // break;


        // }
    }

    if (speed > 0 && (this->gamePlayVars.cloudCount == 0 || this->gamePlayVars.cloudType == CloudType::Flick)) {

        switch (carOffsetOnRoad) {

            case -Constants::OffRoad_Max ... -Constants::OffRoad_BothWheels:

                this->gamePlayVars.cloudCount = Constants::BrakeCloud_OffRoad;
                this->gamePlayVars.cloudSize = Direction::Both;
                this->gamePlayVars.cloudType = CloudType::Normal;
                this->car.setOffroad(true);
                break;

            case -Constants::OffRoad_BothWheels + 1 ... -Constants::OffRoad_OneWheel :

                this->gamePlayVars.cloudCount = Constants::BrakeCloud_OffRoad;
                this->gamePlayVars.cloudSize = Direction::Left;
                this->gamePlayVars.cloudType = CloudType::Normal;
                this->car.setOffroad(true);
                break;

            case Constants::OffRoad_OneWheel  ... Constants::OffRoad_BothWheels - 1:

                this->gamePlayVars.cloudCount = Constants::BrakeCloud_OffRoad;
                this->gamePlayVars.cloudSize = Direction::Right;
                this->gamePlayVars.cloudType = CloudType::Normal;
                this->car.setOffroad(true);
                break;

            case Constants::OffRoad_BothWheels ... Constants::OffRoad_Max:

                this->gamePlayVars.cloudCount = Constants::BrakeCloud_OffRoad;
                this->gamePlayVars.cloudSize = Direction::Both;
                this->gamePlayVars.cloudType = CloudType::Normal;
                this->car.setOffroad(true);
                break;

            default:
                this->car.setOffroad(false);
                break;

        }

    }


    Collision collision;

    if (this->isFrameCount(4)) {

        uint16_t tacho = this->car.getTacho();

        moveCar(0, speed, collision);

        CarMovement carMovement = CarMovement::NoMovement;

        if (!collision.collision) {

            if (!this->car.getOffroad() || this->car.getGear() == 1) {

                if ((PC::buttons.pressed(BTN_A) || PC::buttons.repeat(BTN_A, REPEAT_VAL)) && ((this->car.getGear() == 1 || tacho > 1) && tacho < 8)) {


                    // If accelerating from a stand still, render the dirt cloud ..

                    if (static_cast<uint16_t>(this->car.getSpeed()) == 0) {

                        this->gamePlayVars.cloudCount = Constants::BrakeCloud_Accelerate * 4;
                        this->gamePlayVars.cloudSize = Direction::Both;      
                        this->gamePlayVars.cloudType = CloudType::Normal;

                    }

                    carMovement = CarMovement::Accelerate;

                }
                else if ((PC::buttons.pressed(BTN_A) || PC::buttons.repeat(BTN_A, REPEAT_VAL)) && (tacho == 8 || (this->car.getGear() > 1 && tacho == 1))) {

                    carMovement = CarMovement::NoMovement;

                }
                else {

                    carMovement = CarMovement::Coast;

                }

            }
            else {

                carMovement = CarMovement::Coast;

            }

            if (PC::buttons.released(BTN_A) && (PC::buttons.pressed(BTN_B) || PC::buttons.repeat(BTN_B, REPEAT_VAL))) {

                carMovement = CarMovement::Decelerate;
                
            }

            this->car.changeSpeed(carMovement);

        }
        else {

            if (collision.collision && !collision.nudged) {

                OtherCar &otherCar = otherCars[collision.carIndex];

                if (PC::buttons.pressed(BTN_A) || PC::buttons.repeat(BTN_A, REPEAT_VAL)) {

                    this->car.setSpeed(otherCar.getSpeed());

                    #ifdef DEBUG_COLLISIONS
                        printf("set speed 1\n");
                    #endif
                }
                else {

                    if (!this->car.getOffroad()) {
                        this->car.changeSpeed(CarMovement::NoMovement);
                    }
                    else{
                        this->car.changeSpeed(CarMovement::Coast);
                    }

                }

            }

        }

    }
    else {

        moveCar(0, speed, collision);

        if (collision.collision) {
                    
            OtherCar &otherCar = otherCars[abs(collision.carIndex)];

            if (otherCar.getZ() > this->car.getZ()) {

                this->car.setSpeed(otherCar.getSpeed());

                if (!collision.nudged) {

                    otherCar.setZ(this->car.getZ() + Constants::PlayerCarLengthUnits);
                    #ifdef DEBUG_COLLISIONS
                        printf("set speed 2\n");
                    #endif

                }

            }

        }

    }
    

    // Now, after adjusting the this->car's speed for collisions, move the this->car forward ..

    if ((this->gamePlayVars.showDayBannerCount < 20 && this->gamePlayVars.getDay() == 1) || this->gamePlayVars.getDay() > 1) {

        speed = this->car.getSpeed_Display();

        this->car.setZ(this->car.getZ() + speed);
        this->cameraPos.setZ(this->cameraPos.getZ() + speed);
        this->cameraPos.setY(this->world.roadHeightAt(this->cameraPos.getZ() + 2 * UPM) + UPM);

        moveOtherCars();

    }

}


// ----------------------------------------------------------------------------
// Render the screen .. 
//
void Game::draw(bool drawOtherCars, bool noCurves) {

    Vec3 startPos, endPos;

    uint16_t indexFrom = this->cameraPos.getZ() / UPM;
    uint16_t indexTo = indexFrom + DRAW_DISTANCE; 


    // draw road segments starting with the backmost one first ..
    // IndexTo is the last position and is an ever increasing value.

    if (this->gamePlayVars.oldIndexTo != indexTo + 5) {
        this->world.addRoadSegment(indexTo + 5, noCurves, this->gamePlayVars.getDay(), this->gamePlayVars.getTime(), this->gamePlayVars.getGameLevel());
        this->gamePlayVars.oldIndexTo = indexTo + 5;
    }

    for (uint16_t i = indexTo; i > indexFrom; --i) {

        RoadSegment startVec = this->world.getRoadSegment(i);
        RoadSegment endVec = this->world.getRoadSegment(i + 1);

        startPos = this->world.perspective(startVec, this->cameraPos);
        startPos.setY(startPos.getY() - ((i -1) - indexFrom));
        endPos   = this->world.perspective(endVec, this->cameraPos);
        endPos.setY(endPos.getY() - (i - indexFrom));

        int16_t x1Bottom = startPos.getX() - startPos.getZ() * 1.15;
        int16_t x2Bottom = x1Bottom + startPos.getZ() * 2.3;
        int16_t x1Top = endPos.getX() - endPos.getZ() * 1.15;
        int16_t x2Top = x1Top + endPos.getZ() * 2.3;

        int16_t x1Diff = x1Top - x1Bottom;
        int16_t x2Diff = x2Top - x2Bottom;


        // Draw Horizon ..

        if (i == indexTo) {

            RoadSegment frontMost = this->world.getRoadSegment(indexFrom);
            this->gamePlayVars.horizonX = 64 + frontMost.getX() / 4;

            uint8_t idx = this->gamePlayVars.getTimeOfDay() == TimeOfDay::Day ? 0 : 1;

            for (int16_t i = this->gamePlayVars.horizonX - 256; i < DISPLAY_WIDTH; i = i + 128) {

                PD::drawBitmap(i, endPos.getY() - 9, Images::Horizons[idx]);

            }

        }


        bool odd = i % 2;

        // if visible (ie leaning towards camera) ..

        if (startPos.getY() > endPos.getY() && startPos.getZ() > 0 && endPos.getZ() > 0) {

            int16_t segmentHeight = startPos.getY() - endPos.getY();
            int16_t y = startPos.getY();

            for (int16_t j = 0; j < segmentHeight; ++j) {

                int16_t pos1 = x1Bottom + (j * x1Diff) / segmentHeight;
                int16_t pos2 = x2Bottom + (j * x2Diff) / segmentHeight;

                if (y >= DISPLAY_HEIGHT) {
                    y--;
                    continue;
                }

                pos1 = max<int16_t>(0, pos1);
                pos2 = min<int16_t>(DISPLAY_WIDTH, pos2);

                bool dither = j % 2;
                bool color = y % 2;

                // Odd line
                if (odd) {


                    int16_t k = 0;
                    renderRoadEdge(k, pos1, y, Constants::ColourRoadSide[static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 2] + this->gamePlayVars.getTimeOfDay_PatternForRoad());
                    renderRoad(k, pos2, y, Constants::ColourRoadMain[static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 2] + (this->gamePlayVars.getTimeOfDay_PatternForRoad() / 2));
                    renderRoadEdge(k, DISPLAY_WIDTH, y, Constants::ColourRoadSide[static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 2] + this->gamePlayVars.getTimeOfDay_PatternForRoad());

                }

                // Even line
                else {
                    
                    int16_t k = 0;

                    renderRoadEdge(k, pos1, y, Constants::ColourRoadSide[(static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 2) + 1] + this->gamePlayVars.getTimeOfDay_PatternForRoad());
                    renderRoad(k, pos2, y, Constants::ColourRoadMain[(static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 2) + 1] + (this->gamePlayVars.getTimeOfDay_PatternForRoad() / 2));
                    renderRoadEdge(k, DISPLAY_WIDTH, y, Constants::ColourRoadSide[(static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 2) + 1] + this->gamePlayVars.getTimeOfDay_PatternForRoad());

                }

                y--;
            }


            // Draw lines ..

            if (odd) {

                int16_t pos1Top = x1Bottom + x1Diff;
                int16_t pos2Top = x2Bottom + x2Diff;

                int16_t pBottom = (x2Bottom - x1Bottom) / 6; 
                int16_t pTop = (pos2Top - pos1Top) / 6;

                int16_t centreTop = (x1Top + x2Top) / 2;
                int16_t centreBottom = (x1Bottom + x2Bottom) / 2;

                PD::setColor(4 + (this->gamePlayVars.getTimeOfDay_PatternForRoad() / 2));

                PD::drawLine(centreBottom - pBottom,startPos.getY(),centreTop - pTop,endPos.getY() + 1);

                switch (i - indexFrom) {

                    case 0 ... 2:
                        PD::drawLine(centreBottom - pBottom - 1, startPos.getY(), centreTop - pTop - 1, endPos.getY() + 1);
                        break;

                    case 3 ... 4:
                        PD::drawLine(centreBottom - pBottom - 1, startPos.getY(), centreTop - pTop, endPos.getY() + 1);
                        break;

                    default: break;

                }


                PD::drawLine(centreBottom + pBottom, startPos.getY(), centreTop + pTop, endPos.getY() + 1);

                switch (i - indexFrom) {

                    case 0 ... 2:
                        PD::drawLine(centreBottom + pBottom + 1, startPos.getY(), centreTop + pTop + 1, endPos.getY() + 1);
                        break;

                    case 3 ... 4:
                        PD::drawLine(centreBottom + pBottom + 1, startPos.getY(), centreTop + pTop, endPos.getY() + 1);
                        break;

                    default: break;

                }

            }

        }

        if (startPos.getZ() > 0 && endPos.getZ() > 0) {

            if (startVec.getSegmentType() == RoadSegmentType::Tunnel) {

                RoadSegment tunnelStartVec = startVec.clone();
                RoadSegment tunnelStartVec2 = startVec.clone();

                tunnelStartVec.setY(tunnelStartVec.getY() + 150);
                tunnelStartVec2.setY(tunnelStartVec.getY() + 30);

                Vec3 tunnelStartPos = this->world.perspective(tunnelStartVec, this->cameraPos);
                Vec3 tunnelStartPos2 = this->world.perspective(tunnelStartVec2, this->cameraPos);

                int16_t x1TunnelLeft = tunnelStartPos.getX() - tunnelStartPos.getZ() * 1.55;
                int16_t x2TunnelRight = x1TunnelLeft + tunnelStartPos.getZ() * 3.1;

                int16_t x1PostLeft = tunnelStartPos.getX() - tunnelStartPos.getZ() * 1.45;
                int16_t x2PostLeft = tunnelStartPos.getX() - tunnelStartPos.getZ() * 1.15;

                int16_t x1PostRight = tunnelStartPos.getX() + tunnelStartPos.getZ() * 1.15;
                int16_t x2PostRight = tunnelStartPos.getX() + tunnelStartPos.getZ() * 1.45;

                PD::setColor(Constants::ColourTunnel[i - indexFrom]);
                PD::fillRect(x1TunnelLeft, tunnelStartPos.getY(), x2TunnelRight - x1TunnelLeft,  tunnelStartPos.getY() - tunnelStartPos2.getY());
                PD::fillRect(x1PostLeft, tunnelStartPos.getY() + 2, x2PostLeft - x1PostLeft,  startPos.getY() - tunnelStartPos.getY());
                PD::fillRect(x1PostRight, tunnelStartPos.getY() + 2, x2PostRight - x1PostRight,  startPos.getY() - tunnelStartPos.getY());

            }

        }

    
        if (drawOtherCars) {


            // draw other this->cars ..


            for (uint8_t j = 0; j < this->gamePlayVars.numberOfOtherCars; j++) {

                OtherCar &otherCar = otherCars[j];


                // If the other this->car is within the road segment being rendered then render it ..

                if (otherCar.getZ() > startVec.getZ() && otherCar.getZ() <= endVec.getZ()) {


                    // Render this->cars based on time of day ..
                    
                    uint8_t imageIndex = this->gamePlayVars.getTimeOfDay() == TimeOfDay::Night ? 1 : 0;


                    
                    // Add the average road segement x value to the this->car ..

                    otherCar.setXWorld((startVec.getX() + endVec.getX()) / 2);
                    otherCar.setY(this->world.roadHeightAt(otherCar.getZ()));


                    #ifndef DEBUG_DO_NOT_DRAW_CAR

                        Vec3 otherCarNonRef = otherCar.clone();
                        otherCarNonRef.setX(otherCar.getX() + otherCar.getXWorld());

                        Vec3 carPerspective = this->world.perspective(otherCarNonRef, this->cameraPos);
                        carPerspective.setY(carPerspective.getY() - (i - indexFrom) + 2);
                        int16_t distToCar = otherCar.getZ() - this->cameraPos.getZ();


                        // Only render the this->cars if they are visible ..

                        uint8_t index = 0;

                        if (distToCar > -50 && distToCar < 650) {
                            
                            switch (distToCar) {

                                case 460 ... 650: 
                                    index = 5;
                                    break;

                                case 360 ... 459: 
                                    index = 4;
                                    break;

                                case 270 ... 359: 
                                    index = 3;
                                    break;

                                case 170 ... 269: 
                                    index = 2;
                                    break;

                                case 120 ... 169: 
                                    index = 1;
                                    break;

                                default:
                                    index = 0;
                                    break;

                            }

                            PD::drawBitmap(carPerspective.getX() - (pgm_read_byte(&Images::OtherCar_Width[index]) / 2), carPerspective.getY() - pgm_read_byte(&Images::OtherCar_Height[index]) + 1, Images::OtherCars[index + (imageIndex  * 6)]);

                        }

                    #endif
                    #ifdef DEBUG_COLLISIONS

                        Vec3 leftTopCorner = otherCar.clone();
                        leftTopCorner.setX(leftTopCorner.getX() + otherCar.getXthis->world() - (Constants::OtherCarWidthUnits / 2));
                        leftTopCorner.setZ(leftTopCorner.getZ() + Constants::OtherCarLengthUnits);

                        Vec3 rightTopCorner = otherCar.clone();
                        rightTopCorner.setX(rightTopCorner.getX() + otherCar.getXthis->world() + (Constants::OtherCarWidthUnits / 2));
                        rightTopCorner.setZ(rightTopCorner.getZ() + Constants::OtherCarLengthUnits);
                        
                        Vec3 leftBottomCorner = otherCar.clone();
                        leftBottomCorner.setX(leftBottomCorner.getX() + otherCar.getXthis->world() - (Constants::OtherCarWidthUnits / 2));

                        Vec3 rightBottomCorner = otherCar.clone();
                        rightBottomCorner.setX(rightBottomCorner.getX() + otherCar.getXthis->world() + (Constants::OtherCarWidthUnits / 2));
                        
                        Vec3 leftTopCornerPersepective = this->world.perspective(leftTopCorner, this->cameraPos);
                        Vec3 rightTopCornerPersepective = this->world.perspective(rightTopCorner, this->cameraPos);
                        Vec3 leftBottomCornerPersepective = this->world.perspective(leftBottomCorner, this->cameraPos);
                        Vec3 rightBottomCornerPersepective = this->world.perspective(rightBottomCorner, this->cameraPos);

                        leftTopCornerPersepective.setY(leftTopCornerPersepective.getY() - (i - indexFrom) + 2);
                        rightTopCornerPersepective.setY(rightTopCornerPersepective.getY() - (i - indexFrom) + 2);
                        leftBottomCornerPersepective.setY(leftBottomCornerPersepective.getY() - (i - indexFrom) + 2);
                        rightBottomCornerPersepective.setY(rightBottomCornerPersepective.getY() - (i - indexFrom) + 2);

                        PD::drawLine(leftTopCornerPersepective.getX(), leftTopCornerPersepective.getY(), rightTopCornerPersepective.getX(), rightTopCornerPersepective.getY());
                        PD::drawLine(rightTopCornerPersepective.getX(), rightTopCornerPersepective.getY(), rightBottomCornerPersepective.getX(), rightBottomCornerPersepective.getY());
                        PD::drawLine(rightBottomCornerPersepective.getX(), rightBottomCornerPersepective.getY(), leftBottomCornerPersepective.getX(), leftBottomCornerPersepective.getY());
                        PD::drawLine(leftBottomCornerPersepective.getX(), leftBottomCornerPersepective.getY(), leftTopCornerPersepective.getX(), leftTopCornerPersepective.getY());

                    #endif

                }

            }

        }


        // Draw RoadSide

        if (startVec.getSegmentType() != RoadSegmentType::Tunnel) {

            if (this->gamePlayVars.getScenery() != Scenery::Lights && this->gamePlayVars.getScenery() != Scenery::CornField) {

                for (uint8_t j = 0; j < Constants::NumberOfRoadSide; j++) {

                    Vec3 &roadSidePos = roadSide[j];

                    if (roadSidePos.getZ() > startVec.getZ() && roadSidePos.getZ() <= endVec.getZ()) {

                        uint8_t roadSideIdx = 0;

                        if (i > indexFrom + 2 && i < indexFrom + 4) {
                            roadSideIdx = (static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 5) + 4;
                        }

                        if (i >= indexFrom + 4 && i < indexFrom + 6) {
                            roadSideIdx = (static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 5) + 3;
                        }

                        if (i >= indexFrom + 6 && i < indexFrom + 8) {
                            roadSideIdx = (static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 5) + 2;
                        }

                        if (i >= indexFrom + 8 && i < indexFrom + 10) {
                            roadSideIdx = (static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 5) + 1;
                        }

                        if (i >= indexFrom + 10) {
                            roadSideIdx = (static_cast<uint8_t>(this->gamePlayVars.getScenery()) * 5) + 0;
                        }

                        if (roadSidePos.getX() == 0) {

                            PD::drawBitmap(x1Bottom - 12 - (Images::RoadSide_Width[roadSideIdx] / 2), startPos.getY() - Images::RoadSide_Height[roadSideIdx], Images::RoadSide[roadSideIdx]);

                        }
                        else {

                            PD::drawBitmap(x2Bottom + 13 - (Images::RoadSide_Width[roadSideIdx] / 2), startPos.getY() - Images::RoadSide_Height[roadSideIdx], Images::RoadSide[roadSideIdx]);

                        }

                    }

                }

            }


            // Street lights / rocks ..

            switch (this->gamePlayVars.getScenery()) {

                case Scenery::Desert:
                case Scenery::Snow:
                case Scenery::Tropical:

                    if (this->gamePlayVars.getScenery() != Scenery::Lights && this->gamePlayVars.getScenery() != Scenery::CornField) {

                        if (odd && i > indexFrom + 1) {
                            
                            uint8_t rockIdx = 0;

                            if (i > indexFrom + 2 && i < indexFrom + 4) {
                                rockIdx = 3;
                            }

                            if (i >= indexFrom + 4 && i < indexFrom + 6) {
                                rockIdx = 2;
                            }

                            if (i >= indexFrom + 6 && i < indexFrom + 10) {
                                rockIdx = 1;
                            }

                            if (i >= indexFrom + 10) {
                                rockIdx = 0;
                            }

                            uint8_t spriteSize = Images::Rocks_Width[rockIdx];

                            PD::drawBitmap(x1Bottom - spriteSize, startPos.getY() - spriteSize, Images::Rocks[rockIdx]);
                            PD::drawBitmap(x2Bottom , startPos.getY() - spriteSize, Images::Rocks[rockIdx]);

                        }

                    }

                    break;

                case Scenery::Lights:

                    if (odd && i > indexFrom + 1) {
                        
                        uint8_t postIdx = 0;

                        if (i > indexFrom + 2 && i < indexFrom + 4) {
                            postIdx = 3;
                        }

                        if (i >= indexFrom + 4 && i < indexFrom + 6) {
                            postIdx = 2;
                        }

                        if (i >= indexFrom + 6 && i < indexFrom + 10) {
                            postIdx = 1;
                        }

                        if (i >= indexFrom + 10) {
                            postIdx = 0;
                        }

                        uint8_t spriteSize = Images::Posts_Width[postIdx];

                        PD::drawBitmap(x1Bottom - spriteSize, startPos.getY() - spriteSize, Images::Posts[postIdx]);
                        PD::drawBitmap(x2Bottom , startPos.getY() - spriteSize, Images::Posts[postIdx]);

                    }
                    else {

                        uint8_t lightIdx = 0;

                        if (i < indexFrom + 3)      { lightIdx = 7; }
                        if (i == indexFrom + 3)     { lightIdx = 6; }
                        if (i == indexFrom + 4)     { lightIdx = 5; }
                        if (i == indexFrom + 5)     { lightIdx = 4; }
                        if (i == indexFrom + 6)     { lightIdx = 3; }
                        if (i == indexFrom + 7)     { lightIdx = 2; }
                        if (i == indexFrom + 8)     { lightIdx = 1; }
                        if (i >= indexFrom + 9)     { lightIdx = 0; }

                        uint8_t spriteWidth = Images::StreetLights_Width[lightIdx];
                        uint8_t spriteHeight = Images::StreetLights_Height[lightIdx];                

                        PD::drawBitmap(x1Bottom - spriteWidth, startPos.getY() - spriteHeight, Images::StreetLights_Left[lightIdx]);
                        PD::drawBitmap(x2Bottom, startPos.getY() - spriteHeight, Images::StreetLights_Right[lightIdx]);

                    }

                    break;

                case Scenery::CornField:

                    uint8_t cornIdx = 0;

                    if (i < indexFrom + 3)      { cornIdx = 3; }
                    if (i == indexFrom + 3)     { cornIdx = 3; }
                    if (i == indexFrom + 4)     { cornIdx = 2; }
                    if (i == indexFrom + 5)     { cornIdx = 2; }
                    if (i == indexFrom + 6)     { cornIdx = 1; }
                    if (i == indexFrom + 7)     { cornIdx = 1; }
                    if (i == indexFrom + 8)     { cornIdx = 0; }
                    if (i >= indexFrom + 9)     { cornIdx = 0; }

                    switch (this->gamePlayVars.getTimeOfDay()) {

                        case TimeOfDay::Day:

                            PD::drawBitmap(x1Bottom - 1 - (Images::CornField_Width * 2), startPos.getY() - Images::CornField_Height, Images::CornField_00[cornIdx]);
                            PD::drawBitmap(x1Bottom - 1 -Images::CornField_Width, startPos.getY() - Images::CornField_Height, Images::CornField_00[cornIdx]);
                            PD::drawBitmap(x2Bottom + 1, startPos.getY() - Images::CornField_Height, Images::CornField_00[cornIdx]);
                            PD::drawBitmap(x2Bottom + 1 + Images::CornField_Width, startPos.getY() - Images::CornField_Height, Images::CornField_00[cornIdx]);
                            break;

                        case TimeOfDay::Dawn:

                            PD::drawBitmap(x1Bottom - 1 - (Images::CornField_Width * 2), startPos.getY() - Images::CornField_Height, Images::CornField_01[cornIdx]);
                            PD::drawBitmap(x1Bottom - 1 - Images::CornField_Width, startPos.getY() - Images::CornField_Height, Images::CornField_01[cornIdx]);
                            PD::drawBitmap(x2Bottom + 1, startPos.getY() - Images::CornField_Height, Images::CornField_01[cornIdx]);
                            PD::drawBitmap(x2Bottom + 1 + Images::CornField_Width, startPos.getY() - Images::CornField_Height, Images::CornField_01[cornIdx]);
                            break;

                        case TimeOfDay::Night:

                            PD::drawBitmap(x1Bottom - 1 - (Images::CornField_Width * 2), startPos.getY() - Images::CornField_Height, Images::CornField_02[cornIdx]);
                            PD::drawBitmap(x1Bottom - 1 - Images::CornField_Width, startPos.getY() - Images::CornField_Height, Images::CornField_02[cornIdx]);
                            PD::drawBitmap(x2Bottom + 1, startPos.getY() - Images::CornField_Height, Images::CornField_02[cornIdx]);
                            PD::drawBitmap(x2Bottom + 1 + Images::CornField_Width, startPos.getY() - Images::CornField_Height, Images::CornField_02[cornIdx]);
                            break;
                            
                    }

                    break;

            }

        }

    }

    if (this->gamePlayVars.getScenery() == Scenery::Snow) {

        PD::setColor(4 + (this->gamePlayVars.getTimeOfDay_Pattern() / 2));

        for (uint8_t x = 0; x < Constants::NumberOfSnowFlakes; x++) {

            Vec3 &snowFlake = this->snowFlakes[x];

            if (x % 2 == 0) {

                PD::drawPixel(snowFlake.getX(), snowFlake.getY() / 2);

            }
            else {

                PD::drawRect(snowFlake.getX(), snowFlake.getY() / 2, 1, 1);

            }

        }

    }
  
}
