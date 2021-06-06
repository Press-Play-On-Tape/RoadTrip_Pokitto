#pragma once

// - Debug ------------------------------

#define _DEBUG
#define _DEBUG_COLLISIONS
#define _DEBUG_STRAIGHT_ROAD
#define _DEBUG_LEVEL_ROAD
#define _DEBUG_OTHER_CAR_STEER
#define _DEBUG_DO_NOT_DRAW_CAR


// --------------------------------------

#define DISPLAY_WIDTH 110
#define DISPLAY_HEIGHT 88
#define SOUNDS

#define UPM 64 // units per meter
#define COOKIE_INITIALISED 32
#define REPEAT_VAL 1

#define NO_SCORE 255
#define NO_SOUND 255


namespace Constants {

    constexpr uint8_t NoCollision = 127;
    constexpr uint8_t NumberOfOtherCars = 4;
    constexpr uint8_t NumberOfRoadSide = 3;
    constexpr uint8_t NumberOfSnowFlakes = 100;

    constexpr uint8_t PlayerCarWidthUnits = 54;
    constexpr uint8_t PlayerCarLengthUnits = 50;
    constexpr uint8_t PlayerCarWidthPX = 31;
    constexpr uint8_t PlayerCarHeightPX = 15;
    constexpr uint8_t OtherCarWidthUnits = 42;
    constexpr uint8_t OtherCarLengthUnits = 50;
    constexpr uint8_t CollisionTollerance_X = 65;
    constexpr uint8_t CollisionTollerance_Z = 50;

    constexpr uint16_t SpeedDiv = 1800;//3000;
    constexpr uint16_t ZReduction = 8192;
    constexpr int16_t ZMax = 8192 + 2048;
    constexpr int16_t DistToRenewCars = 50;

    constexpr uint8_t BrakeCloud_Turn = 6;
    constexpr uint8_t BrakeCloud_Accelerate = 10;
    constexpr uint8_t BrakeCloud_DownGear = 10;
    constexpr uint8_t BrakeCloud_OffRoad = 4;

    constexpr uint16_t Gear0Max = 0;
    constexpr uint16_t Gear1Max = 6400;
    constexpr uint16_t Gear2Max = 11200;
    constexpr uint16_t Gear3Max = 21000;
    constexpr uint16_t Gear4Max = 40000;
    constexpr uint16_t Gear5Max = 50000;

    constexpr uint16_t Gear0Min = 0;
    constexpr uint16_t Gear1Min = static_cast<uint16_t>(Gear1Max / 3 * 4);
    constexpr uint16_t Gear2Min = static_cast<uint16_t>(Gear2Max / 3 * 4);
    constexpr uint16_t Gear3Min = static_cast<uint16_t>(Gear3Max / 3 * 4);
    constexpr uint16_t Gear4Min = static_cast<uint16_t>(Gear4Max / 3 * 4);
    constexpr uint16_t Gear5Min = static_cast<uint16_t>(Gear5Max / 3 * 4);

    constexpr uint16_t GearsMin[] = { Gear0Min, Gear1Min, Gear2Min, Gear3Min, Gear4Min, Gear5Min };
    constexpr uint16_t GearsMax[] = { Gear0Max, Gear1Max, Gear2Max, Gear3Max, Gear4Max, Gear5Max };

    constexpr uint8_t  GearboxX[] = { 0, 1, 1, 7, 7, 13 };
    constexpr uint8_t  GearboxY[] = { 0, 1, 13, 1, 13, 1 };

    constexpr uint8_t NewDayBannerDelay = 100;
    constexpr uint8_t NewDayBannerDelay_CountDown = 130;

    constexpr uint8_t CarsToPass_Normal_Init = 30; //30 SJH 
    constexpr uint8_t CarsToPass_Normal_Increase = 5; //5 SJH

    constexpr uint8_t CarsToPass_Easy_Init = 20; 
    constexpr uint8_t CarsToPass_Easy_Increase = 4;
    
    constexpr uint8_t GameOver_MaxOffset = 108;
    constexpr uint8_t Title_MaxOffset = 111;
    constexpr uint8_t NudgeDistance = 10;
    
    constexpr uint16_t OffRoad_Max  = 1000;
    constexpr uint8_t OffRoad_BothWheels  = 139;
    constexpr uint8_t OffRoad_OneWheel  = 103;
    constexpr uint8_t OffRoad_Column_Edge  = 180;
    



    // Sky brightness ..
    
    constexpr uint8_t Sky_BX______D = 6;
    constexpr uint8_t Sky_B_X_____D = 5;
    constexpr uint8_t Sky_B__X____D = 4;
    constexpr uint8_t Sky_B___X___D = 3;
    constexpr uint8_t Sky_B____X__D = 2;
    constexpr uint8_t Sky_B_____X_D = 1;
    constexpr uint8_t Sky_B______XD = 0;


    constexpr uint8_t DoNotEditSlot             = 255;
    constexpr uint8_t MaxNumberOfScores         = 5;

                                       // 00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 
    constexpr uint8_t SpeedSteering[] = {  0,  5,  5,  5,  5,  6,  7,  8,  8,  9,  9, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12  };
//    constexpr uint8_t radioPointerX[] = { 32, 38, 51, 57, 70, 75 };
    constexpr uint8_t radioPointerX[] = { 32, 57, 70 };

    constexpr uint8_t ColourRoadSide[] = { 96, 120, 48, 49, 0, 1, 128, 129, 16, 17 };
    constexpr uint8_t ColourRoadMain[] = { 1, 8, 1, 8, 1, 8, 1, 8, 1, 8 };
    constexpr uint8_t ColourTunnel[] = { 8, 8, 7, 6, 5, 4, 3, 2, 1, 1, 0 };
    constexpr uint8_t ColourWhite = 7;
    constexpr uint8_t ColourBlack = 0;
    constexpr uint8_t ColourTitleBackground = 62;

}

enum class GameState : uint8_t {
    Splash_Init, // 0
    Splash,
    Title_Init,
    Title,
    Game_Init,
    Game,
    GameOver_Init,
    GameOver,
    SaveScore,
    HighScore_Init,
    HighScore,
    Radio_Init,
    Radio
};

enum class Direction : uint8_t {
    None,
    Up,
    Down,
    Left,
    Right,
    Both,
    LeftRight,
    FrontBack,
};

enum class TimeOfDay : uint8_t {
    Day,
    Dawn,
    Night
};

enum class MovementType : uint8_t {
    Horizontal,
    Vertical
};


enum class CarMovement : uint8_t {
    Accelerate,
    Coast,
    Decelerate,
    NoMovement
};

enum class TransmissionType : uint8_t {
    Auto,
    Manual,
};

enum class TitleState : uint8_t {
    StartGame,
    Aim,
    SelectTransmission,
};

enum class SplashScreenState : uint8_t {
    PPOT,
    Gibs,
};

enum class RadioButton : uint8_t {
    None,
    Up,
    Down,
    Off
};

enum class Scenery : uint8_t {
    Desert,
    Tropical,
    Snow,
    Lights,
    CornField,
};

enum class RoadSegmentType : uint8_t {
    Normal,
    Tunnel
};

enum class SoundEffects : uint8_t {
    Auto,   // SD 20
    Manual, // SD 21
    Three,  // SD 22

    Two,    // SD 23
    One,    // SD 24
    Go,     // SD 25

    GameOver,     // SD 26
    Winner,     // SD 27
    Gentlemen,     // SD 28

    HurryUp,
    TimeExtension,
    NewRecord,

    Perfect,

    Bump,           // 11
    Passing,        // 12
    ButtonPress,    // 13
    Beep,           // 14
    Skid,           // 15
};

enum class GameLevel : uint8_t {
    Auto,
    Manual,
    Easy
};

enum class CloudType : uint8_t {
    None,
    Normal,
    Flick
};

enum Themes {
    Radio1,
    Radio2,
    Radio3,
    Intro,
    HighScore,
};