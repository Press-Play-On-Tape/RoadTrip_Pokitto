#pragma once

#include "Pokitto.h"
#include "PokittoCookie.h"
#include "Constants.h"


class GameCookie : public Pokitto::Cookie {

	public:

		uint8_t initialised;
		uint16_t score[3][5] = 	  	{ 50, 40, 30, 20, 10, 50, 40, 30, 20, 10, 50, 40, 30, 20, 10, };
		char initials[3][5][3] =  	{ 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 68, 69, 69, 69, 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 68, 69, 69, 69, 65, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 68, 69, 69, 69, };
		uint16_t lastScore = 0;
		uint16_t lastScoreIndex = 0;
		uint8_t theme;
		bool mute;

	public:

		void initialise() {

			this->initialised = COOKIE_INITIALISED;

			for (uint8_t z = 0; z < 3; z++) {

				this->score[z][0] = 500;
				this->score[z][1] = 400;
				this->score[z][2] = 300;
				this->score[z][3] = 200;
				this->score[z][4] = 100;

				for (uint8_t x = 0; x < 5; x++) {

					for (uint8_t y = 0; y < 3; y++) {

						this->initials[z][x][y] = x;

					}

				}

			}

			this->lastScore = 0;
			this->lastScoreIndex = 0;
			this->theme = 0;
			this->mute = false;
			this->saveCookie();

		}
		
		uint32_t getScoreIndex() {

			return this->lastScoreIndex;

		}

		void setScoreIndex(uint32_t val) {

			this->lastScoreIndex = val;

		}

		uint16_t setLastScore(GameLevel gameLevel, uint32_t score) {

            uint16_t index = NO_SCORE;

			for (uint16_t x = 0; x < 5; x++) {

				if (score >= this->score[static_cast<uint8_t>(gameLevel)][x]) {
				    
				    index = x;
				    break;
				    
				}

			}

			if (index != NO_SCORE) {

			    for (int32_t x = 3; x >= index; x--) {
			        
			        this->score[static_cast<uint8_t>(gameLevel)][x + 1] = this->score[static_cast<uint8_t>(gameLevel)][x];
			        this->initials[static_cast<uint8_t>(gameLevel)][x + 1][0] = this->initials[static_cast<uint8_t>(gameLevel)][x][0];
			        this->initials[static_cast<uint8_t>(gameLevel)][x + 1][1] = this->initials[static_cast<uint8_t>(gameLevel)][x][1];
			        this->initials[static_cast<uint8_t>(gameLevel)][x + 1][2] = this->initials[static_cast<uint8_t>(gameLevel)][x][2];
			        
			    }

				this->score[static_cast<uint8_t>(gameLevel)][index] = score;
				this->initials[static_cast<uint8_t>(gameLevel)][index][0] = 0;
				this->initials[static_cast<uint8_t>(gameLevel)][index][1] = 0;
				this->initials[static_cast<uint8_t>(gameLevel)][index][2] = 0;

			}

			this->lastScore = score;
			this->lastScoreIndex = index;

			return index;

		}

		char getInitial(GameLevel gameLevel, uint8_t slot, uint8_t index) {

			return static_cast<char>(this->initials[static_cast<uint8_t>(gameLevel)][5][3] + 65);
			
		}

		uint16_t getHighScore(GameLevel gameLevel) {

			return this->score[static_cast<uint8_t>(gameLevel)][0];

		}

};