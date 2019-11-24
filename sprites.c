#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sprites.h"
#include "init_video.h"

SprBank load_sprites(SDL_Renderer* rend, char* file, int w, int h){
	SprBank result;
	int width, height;

	SDL_Surface *surface;
	surface = IMG_Load(file);
	result.tex = SDL_CreateTextureFromSurface(rend, surface);
	result.spr_w = w;
	result.spr_h = h;
	SDL_QueryTexture(result.tex, NULL, NULL, &width, &height);
	result.nb_w = width / w;
	SDL_FreeSurface(surface);
	return result;
}

void draw_sprite(SDL_Renderer* r, SprBank s, int n, int x, int y) {
	if ( x >=0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		SDL_Rect src = {
			.x = s.spr_w * (n % s.nb_w), \
			.y = s.spr_h * (n / s.nb_w), \
			.w = s.spr_w, .h = s.spr_h
		};
		SDL_Rect dst = { x, y, s.spr_w, s.spr_h };
		SDL_RenderCopy(r, s.tex, &src, &dst);
	}
}

void draw_string(SDL_Renderer* r, SprBank s, char* str, int x, int y, int sep_pixel) {
	int i = 0;
	while (str[i] != '\0') {
		draw_sprite(r, s, str[i] - 32, x + i * ((s.spr_w) + sep_pixel), y);
		i++;
	}
}