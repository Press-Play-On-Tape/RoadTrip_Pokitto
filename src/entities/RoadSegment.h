#pragma once

#include "Vec3.h"
#include "../utils/Constants.h"
#include "../utils/Curves.h"

struct RoadSegment : Vec3 {

    public:

        RoadSegment() {}

        RoadSegment(int16_t x, int16_t y, int16_t z, RoadSegmentType segmentType) {

            this->setX(x);
            this->setY(y);
            this->setZ(z);
            this->segmentType = segmentType;
            
        }

        RoadSegmentType segmentType = RoadSegmentType::Normal;


    public:

        RoadSegmentType getSegmentType()                    { return this->segmentType; }    

        void setSegmentType(RoadSegmentType val)            { this->segmentType = val; }    

        RoadSegment clone() {

            RoadSegment result;

            result.setX(this->getX());
            result.setY(this->getY());
            result.setZ(this->getZ());
            result.segmentType = this->segmentType;

            return result;

        }

};