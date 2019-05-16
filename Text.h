#pragma once

#include "Render Win.h"
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;

class GameMessage{
public:
	GameMessage(const string &message, const string &font_path, int font_size, const SDL_Color &color);
	~GameMessage();

	void display(int x, int y, int width, int height);
	SDL_Texture *loadFont(const string &message, const string &font_path, int font_size, const SDL_Color &color);

private:
	SDL_Texture *_message_texture;
};
