#include "Functions.h"
#include "Score.h"
#include <iostream>

#define standing 0;
#define jump 0;
#define running1 1;
#define running2 2;
#define crouch_jumping 3;
#define crouch_running1 4;
#define crouch_running2 5;
using namespace std;

GameObject::GameObject(GameWindow &window,const vector<vector<vector<int>>> &rects_info, int x, int y) :
	GameWindow(window), _rects_info(rects_info), _x(x), _y(y)
{}

GameObject::GameObject(GameWindow &window, int w, int h, int x, int y, const string &img_path) :
    GameWindow(window), _w(w), _h(h), _x(x), _y(y) {
        SDL_Surface *surface = IMG_Load(img_path.c_str());
        if (!surface) {
            cerr << "Failed to create surface. " << SDL_GetError() << endl;
        }
        _texture = SDL_CreateTextureFromSurface(GameWindow::_renderer, surface);
        if (!_texture) {
            cerr << "Failed to create texture. " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(surface);
    }

GameObject::~GameObject() {
	SDL_DestroyTexture(_texture);
}


void GameObject::draw() {
	if (_texture) {
		SDL_Rect rect = { _x, _y, _w, _h };
		SDL_RenderCopy(GameWindow::_renderer, _texture, 0, &rect);
		_RECTS.clear();
		_RECTS.push_back(rect);
	} else {
		for (vector<int> i : _rects_info[shape]) {
			SDL_Rect rect =
			{
				i[2] + _x + addition_x,
				i[3] + _y + addition_y,
				i[0], i[1]
			};
			SDL_SetRenderDrawColor(GameWindow::_renderer, 100, 100, 100, 255);
			SDL_RenderFillRect(GameWindow::_renderer, &rect);

			if(_RECTS.size() < _rects_info[shape].size()) {
                _RECTS.push_back(rect);
			} else {
				_RECTS.clear();
				_RECTS.push_back(rect);
			}
		}
	}
}

void GameObject::ground_update() {
	addition_x -= speed + ScoreBoard::current_score / 500;
	if(_rects_info[shape][0][2] + _x + addition_x + _rects_info[shape][0][0] <= 0) {		// outside of the screen
		addition_x = _width - _rects_info[shape][0][2] - _x;
	}
}

void GameObject::obstacle_update() {
	vector<int> appear_rate;
	if(ScoreBoard::current_score <= 1000) {
        appear_rate = { 0,1,1,1,1,2,2,2,2,3,3,3,4};
	} else if(ScoreBoard::current_score <= 2000) {
	 appear_rate = { 0,1,1,2,2,2,2,3,3,3,4,4,4,5,6 };
	} else if(ScoreBoard::current_score <= 4000) {
        appear_rate = { 1,1,2,2,2,3,3,3,3,4,4,4,5,5,6,7,7 };
	} else {
        appear_rate = { 1,2,3,3,3,3,4,4,4,4,5,5,6,6,7,7 };
	}
	addition_x -= speed + ScoreBoard::current_score / 500;
	if(_rects_info[shape][0][2] + _x + addition_x + _rects_info[shape][0][0] <= -300) {	  // outside of the screen
		addition_x = _width - _rects_info[shape][0][2] - _x;
		shape = appear_rate[rand() % appear_rate.size()];
	}
}

int gravity = 30;
double addition_gravity = 0;
double t = 0.1;
int max_velocity = 100;
int velocity = 100;
void GameObject::t_rex_pos_update() {
	if(jumping) {
		if(fast_falling) {
            addition_gravity += 4;
		}
		addition_y = -(velocity * t) + ((gravity + addition_gravity) * t * t / 2);
		t += 0.15;
		if(addition_y >= 0) {
			addition_y = 0;
			addition_gravity = 0;
			t = 0.1;
			jumping = false;
			fast_falling = false;
		}
	}
}

int step = 0;
void GameObject::t_rex_shape_update() {
	if(GameWindow::playing) {
        step++;
	}
	if(GameWindow::playing == false) {
        shape = 0;
	} else if(jumping) {
		if(crouching) {
            shape = 3;
		} else {
            shape = 0;
		}
	} else if(crouching) {
		if(shape != 4 && shape != 5) {
            shape = 4;
        }
		if(step % 7 == 0) {
			if(shape != 4) {
				step = 0;
				shape = 4;
			} else if(shape != 5) {
				step = 0;
				shape = 5;
			}
		}
	} else {
		if(shape != 1 && shape != 2) {
            shape = 1;
		}
		if(step % 7 == 0) {
			if(shape != 1) {
				step = 0;
				shape = 1;
			} else if(shape != 2) {
				step = 0;
				shape = 2;
			}
		}
	}
}

void GameObject::pollEvent(SDL_Event event) {
	switch(event.type) {
        case SDL_MOUSEBUTTONDOWN:
            if(!jumping) {
                jumping = true;
            }
            crouching = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_SPACE) {
                if (!jumping) {
                    jumping = true;
                }
                crouching = false;
                break;
            } else if(event.key.keysym.sym == SDLK_DOWN) {
                if (jumping && !fast_falling) {
                    fast_falling = true;
                    crouching = true;
                } else if(!jumping && !crouching) {
                    crouching = true;
                }
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_DOWN) {
                crouching = false;
            }
            break;
        default:
            break;
    }
}

void GameObject::restart() {
	shape = 0;
	addition_x = 0;
	addition_y = 0;
	jumping = false;
	fast_falling = false;
	crouching = false;
 }
