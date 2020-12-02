#ifndef EVENT_HEADER
#define EVENT_HEADER

#include "../constants/constants.h"
#include "../game/game.h"
#include "graphics.h"
//#ifndef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_ttf.h>
//#endif
#include <stdbool.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

typedef struct emscripten_mainloop {
	SDL_Window** window;
	SDL_Renderer** renderer;
	SDL_Texture** texture;
	TTF_Font** font;
	SDL_Event* event;
	int* ind;
	game** g;
	bool state;
	Uint32 frameStart;
	Uint32 timeCounter; // SHOULD BE =-1
	int time, speed, count, gen, redrawRectangles, changedGameMode;
	int frameTime, CELL;
	coordinates playerView;
	CELL_TYPE ctype;
	
} emscripten_mainloop;

void mainloop(void* args);
#endif

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime);

#endif