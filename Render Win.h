#pragma once


#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
using namespace std;

class GameWindow {
public:
	GameWindow(const string title, int width, int height);
	~GameWindow();

	bool init();
	void pollEvent(SDL_Event event);
	void clear();
	bool isClosed() { return _closed; }
	static bool playing;
	static bool paused;
protected:
	int _width;
	int _height;
private:
	string _title;
	bool _closed = false;

	SDL_Window *_window = nullptr;
public:
	static SDL_Renderer *_renderer;
};
