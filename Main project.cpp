#include "Render Win.h"
#include "Functions.h"
#include "Shapes.h"
#include "Text.h"
#include "Score.h"
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;


void update(GameObject &ground1, GameObject &ground2, GameObject &obstacle1, GameObject &obstacle2, GameObject &t_rex) {
	ground1.ground_update();
	ground2.ground_update();
	obstacle1.obstacle_update();
	obstacle2.obstacle_update();
	t_rex.t_rex_pos_update();
	t_rex.t_rex_shape_update();
}

void scoreBoard_display(GameMessage &sc, GameMessage &hi, ScoreBoard &playerScore) {
	sc.display(770, 10, 50, 20);
	hi.display(770 + 13, 35, 37, 20);
	playerScore.displayScore();
}

void draw(GameObject &ground1, GameObject &ground2, GameObject &obstacle1, GameObject &obstacle2, GameObject &t_rex) {
	ground1.draw();
	ground2.draw();
	obstacle1.draw();
	obstacle2.draw();
	t_rex.draw();
}

void pollEvent(GameWindow &window, GameObject &t_rex) {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		window.pollEvent(event);
		t_rex.pollEvent(event);
	}
}

bool lost_condition(GameObject &t_rex, GameObject &obstacle1, GameObject &obstacle2) {
	for (SDL_Rect i : t_rex._RECTS) {
		for (SDL_Rect j : obstacle1._RECTS)
			if (SDL_HasIntersection(&i, &j))
				return true;
		for (SDL_Rect j : obstacle2._RECTS)
			if (SDL_HasIntersection(&i, &j))
				return true;
	}
	return false;
}

void restart(GameObject &ground1, GameObject &ground2, GameObject &obstacle1, GameObject &obstacle2, GameObject &t_rex, ScoreBoard &playerScore) {
	ground1.restart();
	ground2.restart();
	obstacle1.restart();
	obstacle2.restart();
	t_rex.restart();
	playerScore.clearScore();
	GameWindow::paused = false;
	GameWindow::playing = true;
}

int main(int argc, char* argv[]) {
	srand(time(0));
	int FPS = 60;
	int frameDelay = 1000 / FPS;

	GameWindow window("Dino Run", 900, 400);

	GameObject t_rex(window, t_rex_rects, 0, 115);
	GameObject ground1(window, ground_rects, -100, 200);
	GameObject ground2(window, ground_rects, 800, 200);
	GameObject obstacle1(window, obstacle_rects,700,145);
	GameObject obstacle2(window, obstacle_rects, 700+700, 145);
	GameObject retry_button(window, 80, 80, 400, 160, "texture/retry button.png");

	GameMessage mess("Press SPACE to start", "font/arial.ttf", 100, { 255,0,0,255 });
	GameMessage game_over("GAME OVER!!!!!!!", "font/arial.ttf", 100, { 255,0,0,255 });
	GameMessage tutorial1("UP/SPACE/CLICK: jumping", "font/arial.ttf", 100, { 255,0,0,255 });
	GameMessage tutorial2("DOWN: crouch/fast-falling", "font/arial.ttf", 100, { 255,0,0,255 });
	GameMessage sc("SCORE", "font/arial.ttf", 50, { 255,0,0,255 });
	GameMessage hi("HIGH", "font/arial.ttf", 50, { 255,0,0,255 });

	ScoreBoard playerScore;
	GameMessage digit0("0", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[0] = digit0;
	GameMessage digit1("1", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[1] = digit1;
	GameMessage digit2("2", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[2] = digit2;
	GameMessage digit3("3", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[3] = digit3;
	GameMessage digit4("4", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[4] = digit4;
	GameMessage digit5("5", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[5] = digit5;
	GameMessage digit6("6", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[6] = digit6;
	GameMessage digit7("7", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[7] = digit7;
	GameMessage digit8("8", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[8] = digit8;
	GameMessage digit9("9", "font/arial.ttf", 100, { 255,0,0,255 });		ScoreBoard::score[9] = digit9;

	while (!window.isClosed()) {
		int frameStart = SDL_GetTicks();

		scoreBoard_display(sc, hi, playerScore);
		draw(ground1, ground2, obstacle1, obstacle2, t_rex);
		if (lost_condition(t_rex, obstacle1, obstacle2) == true) {
			GameWindow::paused = true;
			GameWindow::playing = false;
			game_over.display(250, 100, 400, 60);
			retry_button.draw();
		}
		if (GameWindow::playing) {
			update(ground1, ground2, obstacle1, obstacle2, t_rex);
			playerScore.updateScore();
		}
		else {
			if (ScoreBoard::high_score == 0) mess.display(215, 100, 500, 60);
			tutorial1.display(10, 10, 200, 20);
			tutorial2.display(10, 35, 203, 20);
		}

		pollEvent(window, t_rex);
		if (GameWindow::paused == false && lost_condition(t_rex, obstacle1, obstacle2) == true) {
			restart(ground1, ground2, obstacle1, obstacle2, t_rex, playerScore);
		}
		window.clear();


		int frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) SDL_Delay(frameDelay - frameTime);
	}

	return 0;
}
