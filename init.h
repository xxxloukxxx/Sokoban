#ifndef INIT_H_
#define INIT_H_
#include <SDL2/SDL.h>
#include "levels.h"

#define GS_MENU				0x01
#define GS_PLAY				0x02
#define GS_PLAY_NEXT		0x03
#define GS_EXIT				0x04

int game_status;
int current_lvl;
int lvl_max;
char alea[10000];

int get_max_level(void);
void init_params(void);

#endif