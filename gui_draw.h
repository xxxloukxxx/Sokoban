#ifndef GUI_STRINGS_H_
#define GUI_STRINGS_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// Mes includes
#include "init.h"
#include "init_video.h"
#include "sprites.h"
#include "levels.h"
#include "soko.h"
#include "gui.h"

void clear_screen(int r, int g, int b);
void draw_hud();
void draw_level_complete();
void draw_playground();
void draw_ttf_string(SDL_Renderer *r,TTF_Font *f, char* str,int x, int y);
void draw_ui_rect(int x, int y, int w, int h);
void draw_background(int n);
void draw_menu(int choix);
void draw_title();

double f_creneau(double x);

#endif
