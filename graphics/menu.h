#ifndef MENU_DISPLAY
#define MENU_DISPLAY

//#ifndef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_ttf.h>
//#endif
#include "../constants/constants.h"

// Draws the menu
void menu(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, GAME_MODE mode, int ind);

#endif
