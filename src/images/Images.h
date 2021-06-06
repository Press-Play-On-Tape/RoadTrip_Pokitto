#pragma once

namespace Images {

    #include "Banner/Banner.h"
    #include "Banner/Day.h"
    #include "Banner/Goal.h"

    #include "Cactii/Cactii_00.h"
    #include "Cactii/Cactii_01.h"
    #include "Cactii/Cactii_02.h"
    #include "Cactii/Cactii_03.h"
    #include "Cactii/Cactii_04.h"

    #include "Trees/PalmTree_00.h"
    #include "Trees/PalmTree_01.h"
    #include "Trees/PalmTree_02.h"
    #include "Trees/PalmTree_03.h"
    #include "Trees/PalmTree_04.h"

    #include "Trees/PineTree_00.h"
    #include "Trees/PineTree_01.h"
    #include "Trees/PineTree_02.h"
    #include "Trees/PineTree_03.h"
    #include "Trees/PineTree_04.h"

    const uint8_t  RoadSide_Width[] = { 9, 9, 11, 15, 16,      3, 4, 10, 15, 20,    3, 4, 10, 15, 20 };
    const uint8_t  RoadSide_Height[] = { 8, 11, 16, 20, 23,    4, 7, 14, 21, 28,    4, 7, 14, 21, 28 };

    const uint8_t * const RoadSide[] = { 
        Cactii_00,
        Cactii_01,
        Cactii_02,
        Cactii_03,
        Cactii_04,
        PalmTree_00,
        PalmTree_01,
        PalmTree_02,
        PalmTree_03,
        PalmTree_04,
        PineTree_00,
        PineTree_01,
        PineTree_02,
        PineTree_03,
        PineTree_04,
    };

    #include "CarOpp/OppCar_Day_01.h"
    #include "CarOpp/OppCar_Day_02.h"
    #include "CarOpp/OppCar_Day_03.h"
    #include "CarOpp/OppCar_Day_04.h"
    #include "CarOpp/OppCar_Day_05.h"
    #include "CarOpp/OppCar_Day_06.h"
    #include "CarOpp/OppCar_Night_01.h"
    #include "CarOpp/OppCar_Night_02.h"
    #include "CarOpp/OppCar_Night_03.h"
    #include "CarOpp/OppCar_Night_04.h"
    #include "CarOpp/OppCar_Night_05.h"
    #include "CarOpp/OppCar_Night_06.h"

    const uint8_t  OtherCar_Width[] = { 31, 23, 16, 12, 8, 5 };
    const uint8_t  OtherCar_Height[] = { 16, 12, 9, 7, 4, 3 };

    const uint8_t * const OtherCars[] = { 
        OppCar_Day_01,
        OppCar_Day_02,
        OppCar_Day_03,
        OppCar_Day_04,
        OppCar_Day_05,
        OppCar_Day_06,
        OppCar_Night_01,
        OppCar_Night_02,
        OppCar_Night_03,
        OppCar_Night_04,
        OppCar_Night_05,
        OppCar_Night_06
    };

    #include "CarPlayer/PlayerCar_00.h"
    #include "CarPlayer/PlayerCar_01.h"
    #include "CarPlayer/PlayerCar_10.h"
    #include "CarPlayer/PlayerCar_11.h"
    #include "CarPlayer/PlayerTurnLeftCar_00.h"
    #include "CarPlayer/PlayerTurnLeftCar_01.h"
    #include "CarPlayer/PlayerTurnLeftCar_10.h"
    #include "CarPlayer/PlayerTurnLeftCar_11.h"
    #include "CarPlayer/PlayerTurnRightCar_00.h"
    #include "CarPlayer/PlayerTurnRightCar_01.h"
    #include "CarPlayer/PlayerTurnRightCar_10.h"
    #include "CarPlayer/PlayerTurnRightCar_11.h"
   

    const uint8_t * const Player[] = { 
        PlayerCar_00,
        PlayerCar_01,
        PlayerTurnLeftCar_00,
        PlayerTurnLeftCar_01,
        PlayerTurnRightCar_00,
        PlayerTurnRightCar_01,
        PlayerCar_10,
        PlayerCar_11,
        PlayerTurnLeftCar_10,
        PlayerTurnLeftCar_11,
        PlayerTurnRightCar_10,
        PlayerTurnRightCar_11,
    };

    #include "CarPlayer/Dirt_Cloud_Left_00.h"
    #include "CarPlayer/Dirt_Cloud_Left_01.h"
    #include "CarPlayer/Dirt_Cloud_Left_02.h"
    
    const uint8_t * const Dirt_Cloud_Left[] = { 
        Dirt_Cloud_Left_00,
        Dirt_Cloud_Left_01,
        Dirt_Cloud_Left_02
    };

    #include "CarPlayer/Dirt_Cloud_Right_00.h"
    #include "CarPlayer/Dirt_Cloud_Right_01.h"
    #include "CarPlayer/Dirt_Cloud_Right_02.h"
    
    const uint8_t * const Dirt_Cloud_Right[] = { 
        Dirt_Cloud_Right_00,
        Dirt_Cloud_Right_01,
        Dirt_Cloud_Right_02
    };

    #include "CarPlayer/Flick_Left_02.h"
    #include "CarPlayer/Flick_Left_01.h"
    #include "CarPlayer/Flick_Left_00.h"
    
    const uint8_t * const Flick_Cloud_Left[] = { 
        Flick_Left_00,
        Flick_Left_01,
        Flick_Left_02
    };

    #include "CarPlayer/Flick_Right_02.h"
    #include "CarPlayer/Flick_Right_01.h"
    #include "CarPlayer/Flick_Right_00.h"
    
    const uint8_t * const Flick_Cloud_Right[] = { 
        Flick_Right_00,
        Flick_Right_01,
        Flick_Right_02
    };

    #include "GameOver/GameOver_00.h"
    #include "GameOver/GameOver_01.h"

    #include "HUD/CarPassPanel.h"
    #include "HUD/CarPassPanel_Check.h"
    #include "HUD/Gearstick.h"
    #include "HUD/Gearstick_Knob.h"
    #include "HUD/Tacho_00.h"
    #include "HUD/Tacho_01.h"
    #include "HUD/Tacho_02.h"
    #include "HUD/Tacho_03.h"
    #include "HUD/Tacho_04.h"
    #include "HUD/Tacho_05.h"
    #include "HUD/Tacho_06.h"
    #include "HUD/Tacho_07.h"
    #include "HUD/AutoD.h"
    #include "HUD/AutoNumber_01.h"
    #include "HUD/AutoNumber_02.h"
    #include "HUD/AutoNumber_03.h"
    #include "HUD/AutoNumber_04.h"
    #include "HUD/AutoNumber_05.h"
    #include "HUD/Odometer.h"

    const uint8_t * const AutoNumbers[] = { 
        AutoNumber_01,
        AutoNumber_02,
        AutoNumber_03,
        AutoNumber_04,
        AutoNumber_05
    };

    const uint8_t * const Tacho[] = { 
        Tacho_00,
        Tacho_01,
        Tacho_02,
        Tacho_03,
        Tacho_04,
        Tacho_05,
        Tacho_06,
        Tacho_07
    };

    #include "Rock/Rock_00.h"
    #include "Rock/Rock_01.h"
    #include "Rock/Rock_02.h"
    #include "Rock/Rock_03.h"

    const uint8_t Rocks_Width[] = { 3, 5, 7, 10 };

    const uint8_t * const Rocks[] = { 
        Rock_00,
        Rock_01,
        Rock_02,
        Rock_03
    };

    #include "Scenery/Scenery_00.h"
    #include "Scenery/Scenery_01.h"

    const uint8_t * const Horizons[] = { 
        Scenery_00,
        Scenery_01,
    };

    #include "Title/RoadTrip_00.h"
    #include "Title/RoadTrip_01.h"
    #include "Title/Title_Start.h"
    #include "Title/Title_HighScore.h"
    #include "Title/MG.h"
    #include "Title/TransmissionType_Auto.h"
    #include "Title/TransmissionType_Highlight.h"
    #include "Title/TransmissionType_Manual.h"

    #include "PPOT/Ppot_00.h"
    #include "PPOT/Ppot_01.h"
    #include "PPOT/Ppot_02.h"
    #include "PPOT/Ppot_03.h"
    #include "PPOT/Ppot_Arrow.h"
    #include "PPOT/Ppot_Full.h"
    
    const uint8_t * const  Ppot[] = { 
        Ppot_00,
        Ppot_01,
        Ppot_02,
        Ppot_03,
    };

    #include "HighScore/HighScore_Easy.h"
    #include "HighScore/HighScore_Normal.h"
    #include "HighScore/Arrow.h"
    #include "HighScore/Automatic.h"
    #include "HighScore/Manual.h"
    
    const uint8_t * const  HighScore[] = { 
        HighScore_Normal,
        HighScore_Normal,
        HighScore_Easy
    };

    #include "Lights/Lights.h"
    #include "Lights/Light_Green.h"
    #include "Lights/Light_Yellow.h"
    #include "Lights/Light_Red.h"

    #include "Title/Aim.h"
    #include "Title/Corner_BL.h"
    #include "Title/Corner_TL.h"
    #include "Title/Corner_BR.h"
    #include "Title/Corner_TR.h"

    #include "Font4x6/White/Font4x6_White_00.h"
    #include "Font4x6/White/Font4x6_White_01.h"
    #include "Font4x6/White/Font4x6_White_02.h"
    #include "Font4x6/White/Font4x6_White_03.h"
    #include "Font4x6/White/Font4x6_White_04.h"
    #include "Font4x6/White/Font4x6_White_05.h"
    #include "Font4x6/White/Font4x6_White_06.h"
    #include "Font4x6/White/Font4x6_White_07.h"
    #include "Font4x6/White/Font4x6_White_08.h"
    #include "Font4x6/White/Font4x6_White_09.h"
    #include "Font4x6/White/Font4x6_White_10.h"
    #include "Font4x6/White/Font4x6_White_11.h"
    #include "Font4x6/White/Font4x6_White_12.h"
    #include "Font4x6/White/Font4x6_White_13.h"
    #include "Font4x6/White/Font4x6_White_14.h"
    #include "Font4x6/White/Font4x6_White_15.h"
    #include "Font4x6/White/Font4x6_White_16.h"
    #include "Font4x6/White/Font4x6_White_17.h"
    #include "Font4x6/White/Font4x6_White_18.h"
    #include "Font4x6/White/Font4x6_White_19.h"
    #include "Font4x6/White/Font4x6_White_20.h"
    #include "Font4x6/White/Font4x6_White_21.h"
    #include "Font4x6/White/Font4x6_White_22.h"
    #include "Font4x6/White/Font4x6_White_23.h"
    #include "Font4x6/White/Font4x6_White_24.h"
    #include "Font4x6/White/Font4x6_White_25.h"
    #include "Font4x6/White/Font4x6_White_26.h"
    #include "Font4x6/White/Font4x6_White_27.h"
    #include "Font4x6/White/Font4x6_White_28.h"
    #include "Font4x6/White/Font4x6_White_29.h"
    #include "Font4x6/White/Font4x6_White_30.h"
    #include "Font4x6/White/Font4x6_White_31.h"
    #include "Font4x6/White/Font4x6_White_32.h"
    #include "Font4x6/White/Font4x6_White_33.h"
    #include "Font4x6/White/Font4x6_White_34.h"
    #include "Font4x6/White/Font4x6_White_35.h"

    #include "Font4x6/Black/Font4x6_Black_00.h"
    #include "Font4x6/Black/Font4x6_Black_01.h"
    #include "Font4x6/Black/Font4x6_Black_02.h"
    #include "Font4x6/Black/Font4x6_Black_03.h"
    #include "Font4x6/Black/Font4x6_Black_04.h"
    #include "Font4x6/Black/Font4x6_Black_05.h"
    #include "Font4x6/Black/Font4x6_Black_06.h"
    #include "Font4x6/Black/Font4x6_Black_07.h"
    #include "Font4x6/Black/Font4x6_Black_08.h"
    #include "Font4x6/Black/Font4x6_Black_09.h"
    #include "Font4x6/Black/Font4x6_Black_10.h"
    #include "Font4x6/Black/Font4x6_Black_11.h"
    #include "Font4x6/Black/Font4x6_Black_12.h"
    #include "Font4x6/Black/Font4x6_Black_13.h"
    #include "Font4x6/Black/Font4x6_Black_14.h"
    #include "Font4x6/Black/Font4x6_Black_15.h"
    #include "Font4x6/Black/Font4x6_Black_16.h"
    #include "Font4x6/Black/Font4x6_Black_17.h"
    #include "Font4x6/Black/Font4x6_Black_18.h"
    #include "Font4x6/Black/Font4x6_Black_19.h"
    #include "Font4x6/Black/Font4x6_Black_20.h"
    #include "Font4x6/Black/Font4x6_Black_21.h"
    #include "Font4x6/Black/Font4x6_Black_22.h"
    #include "Font4x6/Black/Font4x6_Black_23.h"
    #include "Font4x6/Black/Font4x6_Black_24.h"
    #include "Font4x6/Black/Font4x6_Black_25.h"
    #include "Font4x6/Black/Font4x6_Black_26.h"
    #include "Font4x6/Black/Font4x6_Black_27.h"
    #include "Font4x6/Black/Font4x6_Black_28.h"
    #include "Font4x6/Black/Font4x6_Black_29.h"
    #include "Font4x6/Black/Font4x6_Black_30.h"
    #include "Font4x6/Black/Font4x6_Black_31.h"
    #include "Font4x6/Black/Font4x6_Black_32.h"
    #include "Font4x6/Black/Font4x6_Black_33.h"
    #include "Font4x6/Black/Font4x6_Black_34.h"
    #include "Font4x6/Black/Font4x6_Black_35.h"

    #include "Font4x6/Orange/Font4x6_Orange_26.h"
    #include "Font4x6/Orange/Font4x6_Orange_27.h"
    #include "Font4x6/Orange/Font4x6_Orange_28.h"
    #include "Font4x6/Orange/Font4x6_Orange_29.h"
    #include "Font4x6/Orange/Font4x6_Orange_30.h"
    #include "Font4x6/Orange/Font4x6_Orange_31.h"
    #include "Font4x6/Orange/Font4x6_Orange_32.h"
    #include "Font4x6/Orange/Font4x6_Orange_33.h"
    #include "Font4x6/Orange/Font4x6_Orange_34.h"
    #include "Font4x6/Orange/Font4x6_Orange_35.h"

    const uint8_t * const Font4x6_White[] = { 
        Font4x6_White_00,
        Font4x6_White_01,
        Font4x6_White_02,
        Font4x6_White_03,
        Font4x6_White_04,
        Font4x6_White_05,
        Font4x6_White_06,
        Font4x6_White_07,
        Font4x6_White_08,
        Font4x6_White_09,
        Font4x6_White_10,
        Font4x6_White_11,
        Font4x6_White_12,
        Font4x6_White_13,
        Font4x6_White_14,
        Font4x6_White_15,
        Font4x6_White_16,
        Font4x6_White_17,
        Font4x6_White_18,
        Font4x6_White_19,
        Font4x6_White_20,
        Font4x6_White_21,
        Font4x6_White_22,
        Font4x6_White_23,
        Font4x6_White_24,
        Font4x6_White_25,
        Font4x6_White_26,
        Font4x6_White_27,
        Font4x6_White_28,
        Font4x6_White_29,
        Font4x6_White_30,
        Font4x6_White_31,
        Font4x6_White_32,
        Font4x6_White_33,
        Font4x6_White_34,
        Font4x6_White_35,
    };

    const uint8_t * const Font4x6_Black[] = { 
        Font4x6_Black_00,
        Font4x6_Black_01,
        Font4x6_Black_02,
        Font4x6_Black_03,
        Font4x6_Black_04,
        Font4x6_Black_05,
        Font4x6_Black_06,
        Font4x6_Black_07,
        Font4x6_Black_08,
        Font4x6_Black_09,
        Font4x6_Black_10,
        Font4x6_Black_11,
        Font4x6_Black_12,
        Font4x6_Black_13,
        Font4x6_Black_14,
        Font4x6_Black_15,
        Font4x6_Black_16,
        Font4x6_Black_17,
        Font4x6_Black_18,
        Font4x6_Black_19,
        Font4x6_Black_20,
        Font4x6_Black_21,
        Font4x6_Black_22,
        Font4x6_Black_23,
        Font4x6_Black_24,
        Font4x6_Black_25,
        Font4x6_Black_26,
        Font4x6_Black_27,
        Font4x6_Black_28,
        Font4x6_Black_29,
        Font4x6_Black_30,
        Font4x6_Black_31,
        Font4x6_Black_32,
        Font4x6_Black_33,
        Font4x6_Black_34,
        Font4x6_Black_35,
    };

    const uint8_t * const Font4x6_Orange[] = { 
        Font4x6_Orange_26,
        Font4x6_Orange_27,
        Font4x6_Orange_28,
        Font4x6_Orange_29,
        Font4x6_Orange_30,
        Font4x6_Orange_31,
        Font4x6_Orange_32,
        Font4x6_Orange_33,
        Font4x6_Orange_34,
        Font4x6_Orange_35,
    };

    #include "Font3x5/Black/Font3x5_Black_00.h"
    #include "Font3x5/Black/Font3x5_Black_01.h"
    #include "Font3x5/Black/Font3x5_Black_02.h"
    #include "Font3x5/Black/Font3x5_Black_03.h"
    #include "Font3x5/Black/Font3x5_Black_04.h"
    #include "Font3x5/Black/Font3x5_Black_05.h"
    #include "Font3x5/Black/Font3x5_Black_06.h"
    #include "Font3x5/Black/Font3x5_Black_07.h"
    #include "Font3x5/Black/Font3x5_Black_08.h"
    #include "Font3x5/Black/Font3x5_Black_09.h"

    #include "Font3x5/White/Font3x5_White_00.h"
    #include "Font3x5/White/Font3x5_White_01.h"
    #include "Font3x5/White/Font3x5_White_02.h"
    #include "Font3x5/White/Font3x5_White_03.h"
    #include "Font3x5/White/Font3x5_White_04.h"
    #include "Font3x5/White/Font3x5_White_05.h"
    #include "Font3x5/White/Font3x5_White_06.h"
    #include "Font3x5/White/Font3x5_White_07.h"
    #include "Font3x5/White/Font3x5_White_08.h"
    #include "Font3x5/White/Font3x5_White_09.h"

    const uint8_t * const Font3x5_Black[] = { 
        Font3x5_Black_00,
        Font3x5_Black_01,
        Font3x5_Black_02,
        Font3x5_Black_03,
        Font3x5_Black_04,
        Font3x5_Black_05,
        Font3x5_Black_06,
        Font3x5_Black_07,
        Font3x5_Black_08,
        Font3x5_Black_09,
    };

    const uint8_t * const Font3x5_White[] = { 
        Font3x5_White_00,
        Font3x5_White_01,
        Font3x5_White_02,
        Font3x5_White_03,
        Font3x5_White_04,
        Font3x5_White_05,
        Font3x5_White_06,
        Font3x5_White_07,
        Font3x5_White_08,
        Font3x5_White_09,
    };
    
    #include "Radio/Radio.h"    
    #include "Radio/Radio_Pointer.h"    
    #include "Radio/Radio_Up.h"
    #include "Radio/Radio_Down.h"
    #include "Radio/Radio_Off.h"

    #include "StreetLights/Left/StreetLight_Left_00.h"
    #include "StreetLights/Left/StreetLight_Left_01.h"
    #include "StreetLights/Left/StreetLight_Left_02.h"
    #include "StreetLights/Left/StreetLight_Left_03.h"
    #include "StreetLights/Left/StreetLight_Left_04.h"
    #include "StreetLights/Left/StreetLight_Left_05.h"
    #include "StreetLights/Left/StreetLight_Left_06.h"
    #include "StreetLights/Left/StreetLight_Left_07.h"

    #include "StreetLights/Right/StreetLight_Right_00.h"
    #include "StreetLights/Right/StreetLight_Right_01.h"
    #include "StreetLights/Right/StreetLight_Right_02.h"
    #include "StreetLights/Right/StreetLight_Right_03.h"
    #include "StreetLights/Right/StreetLight_Right_04.h"
    #include "StreetLights/Right/StreetLight_Right_05.h"
    #include "StreetLights/Right/StreetLight_Right_06.h"
    #include "StreetLights/Right/StreetLight_Right_07.h"

    // const uint8_t StreetLights_Width[] = { 3, 3, 5, 7, 10 };
    // const uint8_t StreetLights_Height[] = { 7, 12, 16, 27, 44 };

    const uint8_t StreetLights_Width[] = { 4, 5, 5, 6, 7, 9, 12, 14 };
    const uint8_t StreetLights_Height[] = { 7, 12, 17, 21, 26, 32, 38, 44 };

    const uint8_t * const StreetLights_Left[] = { 
        StreetLight_Left_00,
        StreetLight_Left_01,
        StreetLight_Left_02,
        StreetLight_Left_03,
        StreetLight_Left_04,
        StreetLight_Left_05,
        StreetLight_Left_06,
        StreetLight_Left_07,
    };

    const uint8_t * const StreetLights_Right[] = { 
        StreetLight_Right_00,
        StreetLight_Right_01,
        StreetLight_Right_02,
        StreetLight_Right_03,
        StreetLight_Right_04,
        StreetLight_Right_05,
        StreetLight_Right_06,
        StreetLight_Right_07,
    };

    #include "Posts/Post_00.h"
    #include "Posts/Post_01.h"
    #include "Posts/Post_02.h"
    #include "Posts/Post_03.h"

    const uint8_t Posts_Width[] = { 3, 5, 7, 10 };

    const uint8_t * const Posts[] = { 
        Post_00,
        Post_01,
        Post_02,
        Post_03
    };


    #include "Cornfield/CornField_00_00.h"
    #include "Cornfield/CornField_00_01.h"
    #include "Cornfield/CornField_00_02.h"
    #include "Cornfield/CornField_00_03.h"

    #include "Cornfield/CornField_01_00.h"
    #include "Cornfield/CornField_01_01.h"
    #include "Cornfield/CornField_01_02.h"
    #include "Cornfield/CornField_01_03.h"

    #include "Cornfield/CornField_02_00.h"
    #include "Cornfield/CornField_02_01.h"
    #include "Cornfield/CornField_02_02.h"
    #include "Cornfield/CornField_02_03.h"

    const uint8_t CornField_Width = 55;
    const uint8_t CornField_Height = 15;

    const uint8_t * const CornField_00[] = { 
        CornField_00_00,
        CornField_00_01,
        CornField_00_02,
        CornField_00_03
    };

    const uint8_t * const CornField_01[] = { 
        CornField_01_00,
        CornField_01_01,
        CornField_01_02,
        CornField_01_03
    };

    const uint8_t * const CornField_02[] = { 
        CornField_02_00,
        CornField_02_01,
        CornField_02_02,
        CornField_02_03
    };

}
