#include "Pokitto.h"
#include "src/utils/DuelPalette.h"
#include <LibAudio>
#include "src/RoadTrip.h"
#include "src/utils/GameCookie.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;


Game game;
GameCookie cookie;

Audio::Sink<3, PROJ_AUD_FREQ> audio;

int main(){
    
    cookie.begin("ROADTRIP", sizeof(cookie), (char*)&cookie);

    PC::begin();
    PD::loadRGBPalette(duel);   
    PD::persistence = true;
    PD::setColor(5);
    PD::setInvisibleColor(255);
    PC::setFrameRate(35);
    PD::setFont(fontTight);
    PD::textWrap = false;


    // Has the cookie been initialised?

    if (cookie.initialised != COOKIE_INITIALISED) {

        cookie.initialise();

    }

    game.setup(&cookie);
    
    while( PC::isRunning() ){

        if (!PC::update()) continue;
        game.loop();

    }
    
    return 0;
}
