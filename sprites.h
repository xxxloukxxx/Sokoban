#ifndef SPRITES_H_
#define SPRITES_H_

typedef struct {
	SDL_Texture *tex;
	int spr_w, spr_h;
	int nb_w;
} SprBank;

SprBank load_sprites(SDL_Renderer* renderer, char* file, int w, int h);
void draw_sprite(SDL_Renderer* r, SprBank s, int n, int x, int y);
void draw_string(SDL_Renderer* r, SprBank s, char* str, int x, int y, int sep_pixel);

#endif