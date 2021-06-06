#pragma once

#include "Vec3.h"
#include "../utils/Constants.h"
#include "../utils/Curves.h"

struct Base : Vec3 {

    private:

        float speed = 0;


    public:

        float getSpeed()                    { return this->speed; }    

        void setSpeed(float val)            { this->speed = val; }    

        uint8_t getSpeed_Display() {

            return static_cast<uint8_t>(this->speed / Constants::SpeedDiv);

        }

        void printPos() {

            printf("{");
            printf("%i", this->getX());
            printf(",");
            printf("%i", this->getY());
            printf(",");
            printf("%i", this->getZ());
            printf("}");

        }

};