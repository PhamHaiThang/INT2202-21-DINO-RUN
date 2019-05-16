#pragma once

#include "Render Win.h"
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
using namespace std;

class GameObject : public GameWindow {
public:
	GameObject(GameWindow &window, int w, int h, int x, int y, const string &img_path);
	GameObject(GameWindow &window, const vector<vector<vector<int>>> &rects_info, int x, int y);
	~GameObject();

	vector<SDL_Rect> _RECTS;

	void draw();
	void ground_update();
	void obstacle_update();
	void t_rex_pos_update();
	void t_rex_shape_update();
	void pollEvent(SDL_Event event);
	bool jumping = false;
	bool crouching = false;
	bool fast_falling = false;
	void restart();
private:
	int _w, _h;
	int _x, _y;
	vector<vector<vector<int>>> _rects_info;

	int shape = 0;
	int addition_x = 0;
	int addition_y = 0;

	SDL_Texture *_texture = nullptr;
	int speed = 5;
};
