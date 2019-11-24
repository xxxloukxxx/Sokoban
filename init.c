#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "init.h"
#include "levels.h"


int game_status = GS_MENU;
int current_lvl = 0;
int lvl_max = 1;
char alea[10000];

int get_max_level(void) {
	int i = 10, j = 0;
	while (i > 4) {
		i = 0;
		j++;
		while (levels[j].t[i] != '\0') i++;
	}
	return j;
}

void init_params(void) {
	current_lvl = 0;
	lvl_max = get_max_level();
	srand( time( NULL ) );
	int i;
	for (i = 0; i < 10000; i++) alea[i] = rand() % 100;
}

