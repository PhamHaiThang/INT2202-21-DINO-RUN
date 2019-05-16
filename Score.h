#pragma once
#include "Text.h"
#include <SDL2/SDL_ttf.h>
#include <vector>
using namespace std;

class ScoreBoard {
public:
	void clearScore() { current_score = 0; }
	void updateScore();
	void displayScore();

	static int current_score, high_score;
	static vector<GameMessage> score;
};
