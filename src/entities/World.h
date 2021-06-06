#pragma once

#define ROAD_SEGMENTS 16
#define DRAW_DISTANCE 10

#include <stdint.h>
#include "../utils/Constants.h"

struct World {

    private:

        VecMovement vecHeight = { MovementType::Vertical, Direction::None, static_cast<uint8_t>(random(5, 20)), 0 };
        VecMovement vecTurn = { MovementType::Horizontal, Direction::None, static_cast<uint8_t>(random(5, 20)), 0 };

        RoadSegment roadSegments[ROAD_SEGMENTS] = {

            {0, 0, 0 * UPM, RoadSegmentType::Normal },
            {0, 0, 1 * UPM, RoadSegmentType::Normal },
            {0, 0, 2 * UPM, RoadSegmentType::Normal },
            {0, 0, 3 * UPM, RoadSegmentType::Normal },
            {0, 0, 4 * UPM, RoadSegmentType::Normal },
            {0, 0, 5 * UPM, RoadSegmentType::Normal },
            {0, 0, 6 * UPM, RoadSegmentType::Normal },
            {0, 0, 7 * UPM, RoadSegmentType::Normal },
            {0, 0, 8 * UPM, RoadSegmentType::Normal },
            {0, 0, 9 * UPM, RoadSegmentType::Normal },
            {0, 0, 10 * UPM, RoadSegmentType::Normal },
            {0, 0, 11 * UPM, RoadSegmentType::Normal },
            {0, 0, 12 * UPM, RoadSegmentType::Normal },
            {0, 0, 13 * UPM, RoadSegmentType::Normal },
            {0, 0, 14 * UPM, RoadSegmentType::Normal },
            {0, 0, 15 * UPM, RoadSegmentType::Normal },

        };

    public:

        void print(uint8_t index) {

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                if (i == index) printf("*");

                printf("%i", roadSegments[i].getX());
                printf(",");
                printf("%i", roadSegments[i].getY());
                printf(",");
                printf("%i", roadSegments[i].getZ());
                printf(" ");
                
            }

            printf("\n");

        }
        

        void decZ() {

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                roadSegments[i].setZ(roadSegments[i].getZ() - Constants::ZReduction);
                
            }

        }

        void addRoadSegment(uint8_t index, bool noCurves, uint16_t day, uint16_t timeOfDay, GameLevel gameLevel) {

            uint8_t modOld = index % ROAD_SEGMENTS;
            uint8_t modNew = (index + 1) % ROAD_SEGMENTS;

            RoadSegment &segmentOld = roadSegments[modOld];
            RoadSegment segmentNew = segmentOld.clone();

            segmentNew.setZ(segmentOld.getZ() + UPM);

            #ifndef DEBUG_LEVEL_ROAD
            segmentNew.setY(vecHeight.getMovement());
            #endif

            #ifndef DEBUG_STRAIGHT_ROAD
            if (!noCurves) {
                segmentNew.setX(vecTurn.getMovement());
            }
            #endif

            vecHeight.update();
            vecTurn.update();


            //SJH Tunnel

            switch (gameLevel) {

                case GameLevel::Auto:
                case GameLevel::Manual:

                    if (day % 2 == 0 && day % 8 != 6 && timeOfDay >= 900 && timeOfDay < 1300) {
                        segmentNew.setSegmentType(RoadSegmentType::Tunnel);
                    }
                    else {
                        segmentNew.setSegmentType(RoadSegmentType::Normal);
                    }

                    break;

                case GameLevel::Easy:

                    if (day % 2 == 0 && timeOfDay >= 900 && timeOfDay < 1300) {
                        segmentNew.setSegmentType(RoadSegmentType::Tunnel);
                    }
                    else {
                        segmentNew.setSegmentType(RoadSegmentType::Normal);
                    }

                    break;

            }

            roadSegments[modNew] = segmentNew;

        }

        RoadSegment getRoadSegment(uint16_t index) {
        
            RoadSegment result;
            uint8_t mod = index % ROAD_SEGMENTS;

            result = roadSegments[mod];

            return result;

        }

        RoadSegment getRoadSegment(int16_t zMin, int16_t zMax) {

            RoadSegment result;

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                if (roadSegments[i].getZ() > zMin && roadSegments[i].getZ() <= zMax) {

                    result = roadSegments[i];
                    break;

                }
                
            }

            return result;

        }

        int16_t roadHeightAt(int16_t roadPosition) {

            uint16_t index = roadPosition / UPM;
            int16_t fract = roadPosition % UPM;

            int16_t h1 = getRoadSegment(index).getY();
            int16_t h2 = getRoadSegment(index + 1).getY();

            return h1 + ((h2 - h1) * fract) / UPM;

        }

        // Returns: x, y = screen position, z = screen scale
        Vec3 perspective(Vec3 worldPosition, Vec3 cameraPosition) {

            Vec3 result;
            Vec3 relativeWorldPos = worldPosition - cameraPosition;

            // prevent division by zero
            if (relativeWorldPos.getZ() == 0) relativeWorldPos.setZ(1);

            result.setX(DISPLAY_WIDTH / 2 + (relativeWorldPos.getX() * UPM) / relativeWorldPos.getZ());
            result.setY(DISPLAY_HEIGHT / 2 - (relativeWorldPos.getY() * UPM) / relativeWorldPos.getZ());
            result.setZ((64 * DISPLAY_WIDTH) / relativeWorldPos.getZ());

            return result;

        }

        void reset() {

            for (uint8_t i = 0; i < ROAD_SEGMENTS; i++) {

                this->roadSegments[i].setX(0);
                this->roadSegments[i].setY(0);
                this->roadSegments[i].setZ((i * UPM) - 50);
                this->roadSegments[i].setSegmentType(RoadSegmentType::Normal);
                
            }

            this->vecHeight.setDirection(Direction::None);
            this->vecHeight.setCount(static_cast<uint8_t>(random(5, 20)));

            this->vecTurn.setDirection(Direction::None);
            this->vecTurn.setCount(static_cast<uint8_t>(random(5, 20)));

        }

};