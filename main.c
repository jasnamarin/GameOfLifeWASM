#include <stdbool.h>
#include <string.h>
#include "game/game.h"
#include "graphics/menu.h"
#include "graphics/graphics.h"
#include "graphics/gameWindow.h"
#include "graphics/event.h"

#include <emscripten.h>
#include <SDL_ttf.h>
#include "SDL.h"

#include <stdio.h>

int main() {

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Surface* screen = NULL;
	SDL_Event event;
	SDL_Texture* texture = NULL, *game_screen = NULL;
	TTF_Font* font = NULL;
	game* g = NULL;
	Uint32 frameStart = 0;
	int frameTime = 0;

	// initializing all subsystems
	if (initialize() == false)
		return 1;

	//setting up window, renderer, main screen and font
	if (setup(&window, &renderer, &font, &screen, &texture) == false)
		return 1;

	int ind = 0;
	// setting game mode to begin with
	initGame(&g, MENI);

	printf("before main loop\n");

	// main loop that manages events
	handle_events(&window, &renderer, &texture, &font, &event, &ind, &g, frameStart, frameTime);

	printf("after main loop\n");

	// cleanup
	destroyGame(&g);
	cleanup(&window, &font);

	printf("done");

	return 0;
}
