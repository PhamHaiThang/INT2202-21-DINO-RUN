#include "Render Win.h"
#include <iostream>
#include <vector>
using namespace std;

SDL_Renderer *GameWindow::_renderer;
bool GameWindow::playing = false;
bool GameWindow::paused = true;

GameWindow::GameWindow(string title, int width, int height) :
	_title(title), _width(width), _height(height)
{
	if (!init()) {
		_closed = true;
	}
}

GameWindow::~GameWindow() {
	SDL_DestroyRenderer(GameWindow::_renderer);
	SDL_DestroyWindow(_window);
	IMG_Quit;
	TTF_Quit;
	SDL_Quit;
}

bool GameWindow::init() {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		cerr << "Failed to initalize SDL. " << SDL_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		cerr << "Failed to initalize IMG. " << SDL_GetError() << endl;
		return false;
	}

	if (TTF_Init() == -1) {
		cerr << "Failed to initalizw TTF. " << SDL_GetError() << endl;
		return false;
	}

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, 0);
	if (_window == nullptr) {
		cerr << "Fail to Create Window. " << SDL_GetError() << endl;
		return false;
	}

	GameWindow::_renderer = SDL_CreateRenderer(_window, -1, 0);
	if (GameWindow::_renderer == nullptr) {
		cerr << "Fail to Create Renderer. " << SDL_GetError() << endl;
		return false;
	}

	return true;
};

void GameWindow::pollEvent(SDL_Event event) {
	switch (event.type) {
		case SDL_QUIT:
			_closed = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				paused = false;
				playing = true;
			}
			break;
		case SDL_MOUSEBUTTONDOWN: {
			int mouse_x = event.motion.x;
			int mouse_y = event.motion.y;
			if (playing == false && mouse_x >= 400 && mouse_y >= 160 && mouse_x <= 480 && mouse_y <= 240) {
				paused = false;
				playing = true;
			}
			break;
		}


		default:
			break;
	}
}


void GameWindow::clear() {
	SDL_RenderPresent(GameWindow::_renderer);
	SDL_SetRenderDrawColor(GameWindow::_renderer,200,200,200,255);
	SDL_RenderClear(GameWindow::_renderer);
}
