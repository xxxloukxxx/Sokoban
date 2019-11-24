#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "init_video.h"
#include "gui.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font *font = NULL;


void init_SDL2() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
	}
	window = init_window();
	renderer = init_renderer(window);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();

}

SDL_Window* init_window() {
	SDL_Window* window = SDL_CreateWindow(
	                         WINDOWS_TITLE,
	                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                         SCREEN_WIDTH, SCREEN_HEIGHT,
	                         WINDOWS_FLAGS
	                     );
	if (window == NULL) {
		fprintf(stderr, "could not create window: %s\n", SDL_GetError());
		return NULL;
	} else {
		return window;
	}
}

SDL_Renderer* init_renderer(SDL_Window* window) {
	SDL_Renderer* renderer;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "could not create renderer: %s\n", SDL_GetError());
		return NULL;
	} else {
		return renderer;
	}
}

int quit_SDL2() {
	SDL_DestroyTexture(spr_decor.tex);
	SDL_DestroyTexture(spr_ui.tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	font = NULL;
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}