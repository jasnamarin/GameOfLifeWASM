#include "../constants/constants.h"
#include "../configurations/configurations.h"
#include "../cell/cell.h"
#include "../game/game.h"
#include "../uthash.h"
#include "event.h"
#include "graphics.h"
#include "gameWindow.h"
#include "../configurations/configurations.h"
#include "menu.h"
#include <stdbool.h>

//#ifndef __EMSCRIPTEN__
#include <SDL.h>
#include <SDL_ttf.h>
//#endif

//#ifdef __EMSCRIPTEN__
//EMSCRIPTEN_KEEPALIVE
void mainloop(void* _args){

	emscripten_mainloop* args = _args;

	const int FPS = 30;
	const int frameDelay = 1000 / FPS;
	SDL_Rect returnpos = { WINDOW_W - 50, 10, 30, 30 };
	SDL_Rect playpos = { WINDOW_W - 50, 200, 30, 30 };
	SDL_Rect ctype1pos = { WINDOW_W * 0.45 - 35, WINDOW_H * 0.92, 30, 30 };
	SDL_Rect ctype2pos = { WINDOW_W * 0.45 + 5, WINDOW_H * 0.92, 30, 30 };
	SDL_Rect downloadpos = { WINDOW_W - 50, 50, 30, 30 };
	SDL_Rect uploadpos = { WINDOW_W - 50, 90, 30, 30 };
	SDL_Rect pluspos = { WINDOW_W - 61, 290, 27, 28 };
	SDL_Rect minuspos = { WINDOW_W - 32, 290, 27, 28 };

	if (args->timeCounter == -1)
		args->timeCounter = SDL_GetTicks();
	args->frameStart = SDL_GetTicks();
	while (SDL_PollEvent(args->event) != 0) {
		switch ((*args->event).type) {
		case SDL_QUIT:
			emscripten_cancel_main_loop();
			break;
		// handling keyboard events
		case SDL_KEYDOWN:
			switch ((*args->event).key.keysym.sym) {
			// keys that control menu options
			case SDLK_DOWN:
				if ((*args->g)->gameMode == MENI)
					*args->ind = (*args->ind + 1) % maxChoices;
				else
					down(&args->playerView);
				break;
			case SDLK_UP:
				if ((*args->g)->gameMode == MENI)
					*args->ind = (*args->ind == 0 ? 5 : *args->ind - 1);
				else
					up(&args->playerView);
				break;
			case SDLK_LEFT:
				left(&args->playerView);
				break;
			case SDLK_RIGHT:
				right(&args->playerView);
				break;
			case SDLK_SPACE:
			case SDLK_RETURN:
				args->changedGameMode = 1;
				switch (*args->ind) {
				case 0:
					(*args->g)->gameMode = NORMAL;
					args->count = 1;
					break;
				case 1:
					(*args->g)->gameMode = COEX;
					args->ctype = CELL_COEX_1;
					break;
				case 2:
					(*args->g)->gameMode = PREDATOR;
					break;
				case 3:
					(*args->g)->gameMode = VIRUS;
					break;
				case 4:
					(*args->g)->gameMode = UNKNOWN;
					args->ctype = CELL_COEX_1;
					break;
				case 5:
					emscripten_cancel_main_loop();
					break;
				default:
					break;
				}
				break;
			// key for returning to menu
			case SDLK_ESCAPE:
				destroyGame(args->g);
				args->time = 0;
				initGame(args->g, MENI);
				break;
			case SDLK_p:
				args->state = !args->state;
				args->timeCounter = -1;
				break;
			}
		// handling mouse events
		case SDL_MOUSEBUTTONDOWN:
			if ((*args->event).motion.x < WINDOW_W * 0.95 && (*args->event).motion.y < WINDOW_H * 0.9) {
				int dx = (args->playerView).x;
				int dy = (args->playerView).y;
				int minusX = (dx + (*args->event).motion.x) < 0 ? 1 : 0;
				int minusY = (dy + (*args->event).motion.y) < 0 ? 1 : 0;
				if (getCell(((*args->event).motion.x + dx) / args->CELL - minusX, ((*args->event).motion.y + dy) / args->CELL - minusY, &(*args->g)->table) == CELL_DEAD)
					addCellToGame(((*args->event).motion.x + dx) / args->CELL - minusX, ((*args->event).motion.y + dy) / args->CELL - minusY, args->ctype, *args->g);
				else
					removeCellFromGame(((*args->event).motion.x + dx) / args->CELL - minusX, ((*args->event).motion.y + dy) / args->CELL - minusY, *args->g);
			}
			else {
				if (CheckIfClickedOn(returnpos, (*args->event).motion.x, (*args->event).motion.y)) {
					args->state = 0;
					args->time = 0;
					args->ctype = CELL_NORMAL;
					destroyGame(args->g);
					initGame(args->g, MENI);
					break;
				}
				if (CheckIfClickedOn(playpos, (*args->event).motion.x, (*args->event).motion.y)) {
					args->state = !args->state;
					args->timeCounter = -1;
				}
				if (CheckIfClickedOn(pluspos, (*args->event).motion.x, (*args->event).motion.y)) {
					if (args->gen < 10)
						args->gen++;
				}
				if (CheckIfClickedOn(minuspos, (*args->event).motion.x, (*args->event).motion.y)) {
					if (args->gen > 1)
						args->gen--;
				}
				if (CheckIfClickedOn(downloadpos, (*args->event).motion.x, (*args->event).motion.y))
					loadConfigurationWithList(args->g);
				if (CheckIfClickedOn(uploadpos, (*args->event).motion.x, (*args->event).motion.y))
					saveConfiguration(args->g);
				if (CheckIfClickedOn(ctype1pos, (*args->event).motion.x, (*args->event).motion.y))
					switch ((*args->g)->gameMode)
					{
					case COEX:
					case UNKNOWN:
						args->ctype = CELL_COEX_1;
						break;
					case PREDATOR:
						args->ctype = CELL_PREDATOR;
						break;
					case VIRUS:
						args->ctype = CELL_VIRUS;
						break;
					default:
						break;
					}
				if (CheckIfClickedOn(ctype2pos, (*args->event).motion.x, (*args->event).motion.y))
					switch ((*args->g)->gameMode)
					{
					case COEX:
					case UNKNOWN:
						args->ctype = CELL_COEX_2;
						break;
					case PREDATOR:
						args->ctype = CELL_NORMAL;
						break;
					case VIRUS:
						args->ctype = CELL_NORMAL;
						break;
					default:
						break;
					}
			}
			break;
		// handling mouse wheel events
		case SDL_MOUSEWHEEL:
			// mouse wheel scroll up
			if (args->event->wheel.y > 0)
				zoom_in(&args->CELL, &args->playerView);
			// mouse wheel scroll down
			if (args->event->wheel.y < 0)
				zoom_out(&args->CELL, &args->playerView);
			break;
		default:
			break;
		}
	}

	args->frameTime = SDL_GetTicks() - args->frameStart;
	if (frameDelay > args->frameTime) {
		SDL_Delay(frameDelay - args->frameTime);
	}
	if (SDL_GetTicks() - args->timeCounter >= 1000 && args->state) {
		args->timeCounter = -1;
		args->time++;
	}

	args->count++;
	if (args->state && args->count >= args->speed) {
		evolveByN(*args->g, args->gen);
		args->count = 0;
	}

	// preparing to show events on screen
	SDL_RenderClear(*args->renderer);
	SDL_RenderCopy(*args->renderer, *args->texture, NULL, NULL);

	switch ((*args->g)->gameMode)
	{
	case MENI:
		(args->playerView).x = 0; (args->playerView).y = 0;
		args->CELL = 15;
		menu(*args->window, *args->renderer, *args->font, (*args->g)->gameMode, *args->ind);
		break;
	case NORMAL:
		normal(*args->window, *args->renderer, *args->font, args->g, args->CELL, args->playerView, args->time);
		break;
	case COEX:
		coex(*args->window, *args->renderer, *args->font, args->g, args->CELL, args->playerView, args->time);
		break;
	case PREDATOR:
		predator(*args->window, *args->renderer, *args->font, args->g, args->CELL, args->playerView, args->time);
		break;
	case VIRUS:
		virus(*args->window, *args->renderer, *args->font, args->g, args->CELL, args->playerView, args->time);
		break;
	case UNKNOWN:
		unknown(*args->window, *args->renderer, *args->font, args->g, args->CELL, args->playerView, args->time);
		break;
	default:
		break;
	}

	// showing events on screen
	SDL_RenderPresent(*args->renderer);

}
//#endif

// tracks keyboard events and game state 
void handle_events(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, TTF_Font** font, SDL_Event* event, int* ind, game** g, Uint32 frameStart, int frameTime) {
	
	CELL_TYPE ctype = CELL_NORMAL;
	int CELL = 15;
	int time = 0;
	Uint32 timeCounter = -1;
	coordinates playerView = { 0,0 };
	bool isRunning = true;
	bool state = false; // simulation not running at the moment
	int redrawRectangles = 1;
	int changedGameMode = 0;
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;
	int speed = 2;
	int count = 0;
	int gen = 1;

	#ifdef __EMSCRIPTEN__
	emscripten_mainloop* args = (emscripten_mainloop*)malloc(sizeof(emscripten_mainloop));
	args->window = window;
	args->renderer = renderer;
	args->texture = texture;
	args->font = font;
	args->event = event;
	args->ind = ind;
	args->g = g;
	args->state = false;
	args->frameStart = frameStart;
	args->time = time;
	args->speed = speed;
	args->count = count;
	args->gen = gen;
	args->redrawRectangles = redrawRectangles;
	args->changedGameMode = changedGameMode;
	args->frameTime = frameTime;
	args->CELL = CELL;
	args->timeCounter = timeCounter;
	args->playerView = playerView;
	args->ctype = ctype;

	emscripten_set_main_loop_arg(mainloop, args, 60, 1);

	#else
	SDL_Rect returnpos = { WINDOW_W - 50, 10, 30, 30 };
	SDL_Rect playpos = { WINDOW_W - 50, 200, 30, 30 };
	SDL_Rect ctype1pos = { WINDOW_W * 0.45 - 35, WINDOW_H * 0.92, 30, 30 };
	SDL_Rect ctype2pos = { WINDOW_W * 0.45 + 5, WINDOW_H * 0.92, 30, 30 };
	SDL_Rect downloadpos = { WINDOW_W - 50, 50, 30, 30 };
	SDL_Rect uploadpos = { WINDOW_W - 50, 90, 30, 30 };
	SDL_Rect pluspos = { WINDOW_W - 61, 290, 27, 28 };
	SDL_Rect minuspos = { WINDOW_W - 32, 290, 27, 28 };

	while (isRunning) {
		if (timeCounter == -1)
			timeCounter = SDL_GetTicks();
		frameStart = SDL_GetTicks();
		while (SDL_PollEvent(event) != 0) {
			switch ((*event).type) {
			case SDL_QUIT:
				isRunning = false;
				break;
				// handling keyboard events
			case SDL_KEYDOWN:
				switch ((*event).key.keysym.sym) {
					// keys that control menu options
				case SDLK_DOWN:
					if ((*g)->gameMode == MENI)
						*ind = (*ind + 1) % maxChoices;
					else
						down(&playerView);
					break;
				case SDLK_UP:
					if ((*g)->gameMode == MENI)
						*ind = (*ind == 0 ? 5 : *ind - 1);
					else
						up(&playerView);
					break;
				case SDLK_LEFT:
					left(&playerView);
					break;
				case SDLK_RIGHT:
					right(&playerView);
					break;
        case SDLK_SPACE:
		case SDLK_RETURN:
					changedGameMode = 1;
					switch (*ind) {
					case 0:
						(*g)->gameMode = NORMAL;
						count = 1;
						break;
					case 1:
						(*g)->gameMode = COEX;
						ctype = CELL_COEX_1;
						break;
					case 2:
						(*g)->gameMode = PREDATOR;
						break;
					case 3:
						(*g)->gameMode = VIRUS;
						break;
					case 4:
						(*g)->gameMode = UNKNOWN;
						ctype = CELL_COEX_1;
						break;
					case 5:
						isRunning = false;
						break;
					default:
						break;
					}
					break;
				// key for returning to menu
				case SDLK_ESCAPE:
					destroyGame(g);
					time = 0;
					initGame(g, MENI);
					break;
				case SDLK_p:
					state = !state;
					timeCounter = -1;
					break;
				}
			// handling mouse events
			case SDL_MOUSEBUTTONDOWN:
				if ((*event).motion.x < WINDOW_W*0.95 && (*event).motion.y < WINDOW_H*0.9) {
					int dx = playerView.x;
					int dy = playerView.y;
					int minusX = (dx + (*event).motion.x) < 0 ? 1 : 0;
					int minusY = (dy + (*event).motion.y) < 0 ? 1 : 0;
					if (getCell(((*event).motion.x + dx)/ CELL - minusX, ((*event).motion.y + dy ) / CELL - minusY, &(*g)->table) == CELL_DEAD)
						addCellToGame(((*event).motion.x + dx)/ CELL - minusX, ((*event).motion.y + dy)/ CELL - minusY, ctype, *g);
					else
						removeCellFromGame(((*event).motion.x + dx) / CELL - minusX, ((*event).motion.y + dy) / CELL - minusY, *g);
				}else {
					if (CheckIfClickedOn(returnpos, (*event).motion.x, (*event).motion.y)) {
						state = 0;
						time = 0;
						ctype = CELL_NORMAL;
						destroyGame(g);
						initGame(g, MENI);
						break;
					}
					if (CheckIfClickedOn(playpos, (*event).motion.x, (*event).motion.y)) {
						state = !state;
						timeCounter = -1;
					}
					if (CheckIfClickedOn(pluspos, (*event).motion.x, (*event).motion.y)) {
						if (gen < 10)
							gen++;
					}
					if (CheckIfClickedOn(minuspos, (*event).motion.x, (*event).motion.y)) {
						if (gen > 1)
							gen--;
					}
					if (CheckIfClickedOn(downloadpos, (*event).motion.x, (*event).motion.y))
						loadConfigurationWithList(g);
					if (CheckIfClickedOn(uploadpos, (*event).motion.x, (*event).motion.y))
						saveConfiguration(g);
					if (CheckIfClickedOn(ctype1pos, (*event).motion.x, (*event).motion.y))
						switch ((*g)->gameMode)
						{
						case COEX:
						case UNKNOWN:
							ctype = CELL_COEX_1;
							break;
						case PREDATOR:
							ctype = CELL_PREDATOR;
							break;
						case VIRUS:
							ctype = CELL_VIRUS;
							break;
						default:
							break;
						}
					if (CheckIfClickedOn(ctype2pos, (*event).motion.x, (*event).motion.y))
						switch ((*g)->gameMode)
						{
						case COEX:
						case UNKNOWN:
							ctype = CELL_COEX_2;
							break;
						case PREDATOR:
							ctype = CELL_NORMAL;
							break;
						case VIRUS:
							ctype = CELL_NORMAL;
							break;
						default:
							break;
						}
				}
				break;
			// handling mouse wheel events
			case SDL_MOUSEWHEEL:
				// mouse wheel scroll up
				if (event->wheel.y > 0)
					zoom_in(&CELL, &playerView);
				// mouse wheel scroll down
				if (event->wheel.y < 0)
					zoom_out(&CELL, &playerView);
				break;
			default:
				break;
			}	
		}

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
		if (SDL_GetTicks() - timeCounter >= 1000 && state) {
			timeCounter = -1;
			time++;
		}

		count++;
		if (state && count >= speed) {
			evolveByN(*g, gen);
			count = 0;
		}	

		// preparing to show events on screen
		SDL_RenderClear(*renderer);
		SDL_RenderCopy(*renderer, *texture, NULL, NULL);

		switch ((*g)->gameMode)
		{
		case MENI:
			playerView.x = 0; playerView.y = 0;
			CELL = 15;
			menu(*window, *renderer, *font, (*g)->gameMode, *ind);
			break;
		case NORMAL:
			normal(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		case COEX:
			coex(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		case PREDATOR:
			predator(*window, *renderer, *font,  g, CELL, playerView, time);
			break;
		case VIRUS:
			virus(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		case UNKNOWN:
			unknown(*window, *renderer, *font, g, CELL, playerView, time);
			break;
		default:
			break;
		}
		
		// showing events on screen
		SDL_RenderPresent(*renderer);
	}
	#endif

}
