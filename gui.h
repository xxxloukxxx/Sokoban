#ifndef GUI_H_
#define GUI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "sprites.h"
#include "levels.h"

#define DELAY 25



// Variables globales
int game_status, number_of_moves, lvl_number, move_ok, lvl_running;
SprBank	spr_decor,spr_ui;
char direction;
Tlevel lvl;
TTF_Font *font;
SDL_Texture *logo;

void init_gui();
int play();
int menu();

#endif