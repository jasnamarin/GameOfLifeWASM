#include <stdbool.h>
#include "graphics.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <stdio.h>

//initializes all subsystems
bool initialize() {

	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		return false;

	// initialize TTF
	if (TTF_Init() < 0)
		return false;

	return true;
}

//sets up window, renderer, main screen and font
bool setup(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font, SDL_Surface** screen, SDL_Texture** texture) {

	// loading font
	*font = TTF_OpenFont("graphics/gamecontinue.ttf", 25);
	if (*font == NULL) {
		printf("could not find font\n");
		return false;
	}

	printf("create window\n");

	// creating window and renderer
	*window = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE | SDL_RENDERER_ACCELERATED);
	if (*window == NULL)
		return false;

	printf("create renderer\n");

	*renderer = SDL_CreateRenderer(*window, -1, 0);
	if (*renderer == NULL)
		return false;

	printf("set up main screen\n");

	//setting up main screen
	*screen = SDL_GetWindowSurface(*window);
	SDL_SetSurfaceColorMod(*screen, 0, 0, 0);
	*texture = SDL_CreateTextureFromSurface(*renderer, *screen);
	if (*screen == NULL || *texture == NULL)
		return false;

	printf("setup all done\n");

	return true;
}

// closes all subsystems
void cleanup(SDL_Window** window, TTF_Font** font) {
	SDL_DestroyWindow(*window);
	TTF_CloseFont(*font);
	TTF_Quit();
	SDL_Quit();
}

button InitButton( int x, int y, int width, int height) {
	struct Button button;
	button.texture = NULL;
	button.position.x = x;
	button.position.y = y;
	button.position.w = width;
	button.position.h = height;

	return button;
}

void clearButton(button* b) {
	SDL_DestroyTexture(b->texture);
}

bool CheckIfClickedOn(SDL_Rect position, int mouse_x, int mouse_y) {
	if (position.x <= mouse_x && position.x + position.w >= mouse_x && position.y <= mouse_y && position.y + position.h >= mouse_y)
		return true;
	return false;
}
